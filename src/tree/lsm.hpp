//
// Created by cynder on 21.06.19.
//

#pragma once

#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

#include "../string/str.h"
#include "../container/vector.hpp"
#include "../struct/optional.hpp"
#include "../tree/avl.hpp"
#include "../struct_algo/binary_search.hpp"
#include "../template/arraymethods.hpp"
#include "../other/serializable.hpp"

namespace
{
    unsigned char file_type(const str &name, ino_t &inode)  //0 - not exists, 1 - regular file, 2 - directory, 3 - something else
    {
        struct stat buffer; //NOLINT (will be initialized in stat)
        if(stat(name.c_str(), &buffer))
            return 0;
        else
        {
            inode = buffer.st_ino;
            if(buffer.st_mode & S_IFREG)
                return 1;
            elif(buffer.st_mode & S_IFDIR)
                return 2;
            else
                return 3;
        }
    }

    inline unsigned char file_type(const str &name)
    {
        ino_t ignored;
        return file_type(name, ignored);
    }
}

template<typename TKey, typename TData>
struct sstable
{
    struct record
    {
        TKey key;
        uint64_t timestamp;
        str path;
        size_t offset, size;
    };
    str file_written;
    record* index;
    size_t index_length;
    TData *data;

    sstable(): index(nullptr), index_length(0), data(nullptr)
    {}

    sstable(const sstable &otr): file_written(otr.file_written), index(otr.index), index_length(otr.index_length),
                                 data(otr.data)
    {}

    sstable(avl_tree<std::pair<uint64_t, TData>, TKey> &c0)
    {
        index_length = c0.size();
        index = new record[index_length];
        data = new TData[index_length];
        size_t pos = 0;
        for(auto it = c0.begin();it != c0.end();++it)
        {
            index[pos] = {it.key(), (*it).first, str(), pos, 0};
            data[pos] = (*it).second;
        }
        c0 = avl_tree<std::pair<uint64_t, TData>, TKey>();
    }

    ~sstable()
    {}

    void merge(const sstable &otr)
    {
        record *new_index = new record[index_length + otr.index_length];
        size_t i1 = 0, i2 = 0, i3 = 0;
        if(index_length > 0 && otr.index_length > 0)
            while(true)
            {
                if(index[i1].key < otr.index[i2].key)
                {
                    new_index[i3++] = index[i1];
                    if(++i1 == index_length)
                        break;
                }
                else if(otr.index[i2].key < index[i1].key)
                {
                    new_index[i3++] = otr.index[i2];
                    if(++i2 == otr.index_length)
                        break;
                }
                else
                {
                    if(index[i1].timestamp < otr.index[i2].timestamp)
                        new_index[i3] = otr.index[i2];
                    else
                        new_index[i3] = index[i1];
                    i1++, i2++, i3++;
                    if(i1 == index_length || i2 == otr.index_length)
                        break;
                }
            }
        if(i1 < index_length)
        {
            _copy(index + i1, index + index_length, new_index + i3);
            i3 += index_length - i1;
        }
        else if(i2 < otr.index_length)
        {
            _copy(otr.index + i2, otr.index + otr.index_length, new_index + i3);
            i3 += otr.index_length - i2;
        }
        delete[] index;
        index = new_index;
        index_length = i3;
    }

    void write(str dir, unsigned file_id = std::numeric_limits<unsigned>::max())
    {
        str name = dir + "/" + get_name(file_id);
        str table_name = name + ".sst";
        str index_name = name + ".ssi";
        FILE *table = fopen(table_name.c_str(), "wb");
        size_t current_offset = 0;
        for(size_t i = 0;i < index_length;i++)
        {
            serialized s;
            if(index[i].path.length() == 0)
                s.serialize(data[index[i].offset]);
            else
                s.read(index[i].path, index[i].offset, index[i].size);
            fwrite(s.data(), 1, s.size(), table);
            index[i].path = table_name;
            index[i].offset = current_offset;
            index[i].size = s.size();
            current_offset += s.size();
        }
        fclose(table);
        file_written = name;
        FILE *indx = fopen(index_name.c_str(), "wb");
        serialized s = serialize();
        fwrite(s.data(), 1, s.size(), indx);
        fclose(indx);
        delete[] data;
        data = nullptr;
    }

    void clear()
    {
        str table_name = file_written + ".sst";
        str index_name = file_written + ".ssi";
        remove(table_name.c_str());
        remove(index_name.c_str());
        delete[] index;
        index = nullptr;
    }

    size_t size() const
    {
        return index_length;
    }

    optional<TData> get(TKey key)
    {
        binary_search<record*> bs(index, index + index_length);
        bs.template lower_bound<TKey, _less<TKey>, _get>(key);
        if(bs.done())
            return optional<TData>();
        size_t pos = bs.current() - index;
        if(index[pos].path.length() == 0)
            return optional<TData>(data[index[pos].offset]);
        serialized s;
        s.read(index[pos].path, index[pos].offset, index[pos].size);
        return optional<TData>(s.deserialize<TData>());
    }

    serialized serialize()
    {
        size_t buffer_length = file_written.length() + 1 + sizeof(size_t) +
                               index_length * (sizeof(TKey) + sizeof(uint64_t) + sizeof(size_t));
        unsigned char *buffer = new unsigned char[buffer_length];
        memcpy(buffer, file_written.c_str(), file_written.length() + 1);
        size_t offset = file_written.length() + 1;
        memcpy(buffer + offset, &index_length, sizeof(size_t));
        offset += sizeof(size_t);
        for(size_t i = 0;i < index_length;i++)
        {
            memcpy(buffer + offset, &index[i].key, sizeof(TKey));
            offset += sizeof(TKey);
            memcpy(buffer + offset, &index[i].timestamp, sizeof(uint64_t));
            offset += sizeof(uint64_t);
            memcpy(buffer + offset, &index[i].offset, sizeof(size_t));
            offset += sizeof(size_t);
        }
        return serialized(buffer, buffer_length);
    }

    static sstable deserialize(unsigned char *buffer, size_t buffer_length)
    {
        sstable s;
        size_t offset = 0;
        while(buffer[offset])
            offset++;
        s.file_written = str(reinterpret_cast<char*>(buffer), offset++);
        str full_path = s.file_written + ".sst";
        memcpy(&s.index_length, buffer + offset, sizeof(size_t));
        s.index = new record[s.index_length];
        offset += sizeof(size_t);
        size_t i = 0;
        while(offset < buffer_length)
        {
            memcpy(&s.index[i].key, buffer + offset, sizeof(TKey));
            offset += sizeof(TKey);
            memcpy(&s.index[i].timestamp, buffer + offset, sizeof(uint64_t));
            offset += sizeof(uint64_t);
            s.index[i].path = full_path;
            memcpy(&s.index[i].offset, buffer + offset, sizeof(size_t));
            offset += sizeof(size_t);
            i++;
        }
        for(size_t i = 1;i < s.index_length;i++)
            s.index[i - 1].size = s.index[i].offset - s.index[i - 1].offset;
        if(s.index_length)
        {
            FILE *f = fopen(full_path.c_str(), "rb");
            fseek(f, 0, SEEK_END);
            s.index[s.index_length - 1].size = ftell(f);
            if(s.index_length > 1)
                s.index[s.index_length - 1].size -= s.index[s.index_length - 2].offset;
            fclose(f);
        }
        return s;
    }

    static str get_name(unsigned file_id = std::numeric_limits<unsigned>::max())
    {
        return str("lsm_sstable.") + (file_id != std::numeric_limits<unsigned>::max() ? str(file_id) : "f");
    }

    static TKey _get(record *r)
    {
        return r->key;
    }
};

template<typename TKey, typename TData>
struct lsm
{
    str dir;
    vect<sstable<TKey, optional<TData>>> index;
    avl_tree<std::pair<uint64_t, optional<TData>>, TKey> c0;
    size_t limit_size;
    uint64_t timestamp;

    lsm(size_t c0_size = 1024, const char* save_path = "./"): dir(str(save_path) + "/"), limit_size(c0_size), timestamp(0)
    {
        ASSERT(limit_size > 0);
        ASSERT(file_type(save_path) == 2);
        str final_indx = dir + sstable<TKey, optional<TData>>::get_name() + ".ssi";
        str final_tble = dir + sstable<TKey, optional<TData>>::get_name() + ".sst";
        str indx_0 = dir + sstable<TKey, optional<TData>>::get_name(0) + ".ssi";
        str tble_0 = dir + sstable<TKey, optional<TData>>::get_name(0) + ".sst";
        if(file_type(final_indx.c_str()) == 1)
        {
            serialized s;
            s.read(final_indx);
            auto final = s.deserialize<sstable<TKey, optional<TData>>>();
            final.write(dir, 0);
            rename(final_tble.c_str(), tble_0.c_str());
            ::remove(final_indx.c_str());
        }
        if(file_type(indx_0.c_str()) == 1)
        {
            for(size_t i = 0;;i++)
            {
                str indx = dir + sstable<TKey, optional<TData>>::get_name(i) + ".ssi";
                if(file_type(indx.c_str()) != 1)
                    break;
                serialized s;
                s.read(indx);
                index.push(s.deserialize<sstable<TKey, optional<TData>>>());
            }
            for(size_t i = 0;i < index[index.size() - 1].size();i++)
                if(index[index.size() - 1].index[i].timestamp >= timestamp)
                    timestamp = index[index.size() - 1].index[i].timestamp + 1;
        }
    }

    ~lsm()
    {
        auto last = sstable<TKey, optional<TData>>(c0);
        for(size_t i = index.size();i > 0;)
            last.merge(index[--i]);
        last.write(dir);
        for(auto it : index)
            it.clear();
        last.write(dir, 0);
        str final_indx = dir + sstable<TKey, optional<TData>>::get_name() + ".ssi";
        str final_tble = dir + sstable<TKey, optional<TData>>::get_name() + ".sst";
        str tble_0 = dir + sstable<TKey, optional<TData>>::get_name(0) + ".sst";
        rename(final_tble.c_str(), tble_0.c_str());
        ::remove(final_indx.c_str());
    }

    void insert(TKey key, TData data)
    {
        c0.insert(key, std::make_pair(timestamp++, optional<TData>(data)), true);
        if(c0.size() == limit_size)
        {
            auto t = sstable<TKey, optional<TData>>(c0);
            t.write(dir, index.size());
            index.push(t);
        }
    }

    optional<TData> get(TKey key)
    {
        auto it = c0.findKey(key);
        if(it != c0.end())
            return (*it).second;
        for(size_t i = index.size();i > 0;)
        {
            auto obj = index[--i].get(key);
            if(obj)
                return *obj;
        }
        return optional<TData>();
    }

    void remove(TKey key)
    {
        c0.insert(key, std::make_pair(timestamp++, optional<TData>()), true);
        if(c0.size() == limit_size)
        {
            auto t = sstable<TKey, optional<TData>>(c0);
            t.write(dir, index.size());
            index.push(t);
        }
    }
};