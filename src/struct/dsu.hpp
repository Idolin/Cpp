#pragma once

#include "../other/rand.h"
#include "../container/vector.hxx"

#include <algorithm>

using std::min;
using std::max;
using std::pair;
using std::make_pair;

struct dsu
{
    vect <pair<unsigned, unsigned>> s;

    dsu(unsigned count = 0) : s(vect<pair<unsigned, unsigned>>(count))
    {
        for(unsigned i = 0; i < count; i++)
            s[i] = make_pair(i, 1);
    }

    ~dsu()
    {}

    void add(unsigned count = 1)
    {
        unsigned endc = s.maxs + count;
        for(unsigned i = s.maxs; i < endc; i++)
            s[i] = make_pair(i, 1);
    }

    unsigned get(unsigned p)
    {
        if(p < s.maxs)
        {
            unsigned t = s[p].first;
            if(s[t].first != t)
                s[p] = make_pair(this->get(t), 1);
            return s[s[p].first].first;
        }
        else
        {
            this->add(p - s.maxs + 1);
            return p;
        }
    }

    void unite(unsigned f, unsigned sc)
    {
        unsigned fr = this->get(f);
        unsigned sr = this->get(sc);
        if(fr != sr)
        {
            if(s[fr].second >= s[sr].second)
            {
                if(s[fr].second == s[sr].second)
                    s[fr] = make_pair(fr, s[fr].second + 1);
                s[sr] = make_pair(fr, 0);
            }
            else
                s[fr] = make_pair(sr, 0);
        }
    }

    bool sameSet(unsigned f, unsigned s)
    {
        return this->get(f) == this->get(s);
    }
};

struct dsur
{
    vect<unsigned> s;

    dsur(unsigned count = 0) : s(vect<unsigned>(count))
    {
        for(unsigned i = 0; i < count; i++)
            s[i] = i;
    }

    ~dsur()
    {}

    void add(unsigned count = 1)
    {
        unsigned endc = s.maxs + count;
        for(unsigned i = s.maxs; i < endc; i++)
            s[i] = i;
    }

    unsigned get(unsigned p)
    {
        if(p < s.maxs)
        {
            unsigned t = s[p];
            if(s[t] != t)
                s[p] = this->get(t);
            return s[s[p]];
        } else
        {
            this->add(p - s.maxs + 1);
            return p;
        }
    }

    void unite(unsigned f, unsigned sc)
    {
        unsigned fr = this->get(f);
        unsigned sr = this->get(sc);
        if(fr != sr)
        {
            if(randomB())
                s[sr] = fr;
            else
                s[fr] = sr;
        }
    }

    bool sameSet(int f, int s)
    {
        return this->get(f) == this->get(s);
    }
};

struct dsus
{
    vect <pair<unsigned, unsigned>> s;

    dsus(unsigned count = 0) : s(vect<pair<unsigned, unsigned>>(count))
    {
        if(count-- > 0)
        {
            s[count] = make_pair(count, count);
            for(unsigned i = 0; i < count; i++)
                s[i] = make_pair(i, i);
        }
    }

    ~dsus()
    {}

    void add(unsigned count = 1)
    {
        if(count-- > 0)
        {
            unsigned endc = s.maxs + count;
            s[endc] = make_pair(endc, endc);
            for(unsigned i = s.maxs; i < endc; i++)
                s[i] = make_pair(i, i);
        }
    }

    unsigned get(unsigned p)
    {
        if(p < s.maxs)
        {
            unsigned t = s[p].first;
            if(s[t].first != t)
                s[p] = make_pair(this->get(t), s[p].second);
            return s[s[p].first].first;
        } else
        {
            this->add(p - s.maxs + 1);
            return p;
        }
    }

    void unite(unsigned f, unsigned sc)
    {
        unsigned fr = this->get(f);
        unsigned sr = this->get(sc);
        if(fr != sr)
        {
            if(randomB())
                fr = sr;
            s[fr] = make_pair(sr, s[fr].second);
        }
    }

    void uniterange(unsigned f, unsigned sc)
    {
        if(sc >= s.maxs)
            this->add(sc - s.maxs + 1);
        unsigned tr = f, tr2;
        while(s[s[tr].first].second != s[s[sc].first].second)
        {
            this->unite(tr, sc);
            tr2 = s[s[tr].first].second + 1;
            s[s[tr].first] = make_pair(s[s[tr].first].first, sc);
            tr = tr2;
            if(tr >= s.maxs)
                break;
        }
    }

    bool sameSet(unsigned f, unsigned s)
    {
        return this->get(f) == this->get(s);
    }
};

struct dsumma
{
    vect<unsigned *> s;

    dsumma(unsigned count = 0) : s(vect<unsigned *>(count))
    {
        for(unsigned i = 0; i < count; i++)
        {
            s[i] = (unsigned *) malloc(sizeof(unsigned) * 5);
            s[i][0] = i; //pointer
            s[i][1] = 0; //maxpossiblelength
            s[i][2] = i; //min
            s[i][3] = i; //max
            s[i][4] = 1; //counter
        }
    }

    ~dsumma()
    {
        for(unsigned i = 0; i < s.maxs; i++)
            free(s[i]);
    }

    void add(unsigned count = 1)
    {
        unsigned endc = s.maxs + count;
        for(unsigned i = s.maxs; i < endc; i++)
        {
            s[i] = (unsigned *) malloc(sizeof(unsigned) * 5);
            s[i][0] = i; //pointer
            s[i][1] = 0; //maxpossiblelength
            s[i][2] = i; //min
            s[i][3] = i; //max
            s[i][4] = 1; //counter
        }
    }

    unsigned *get(unsigned p)
    {
        if(p < s.maxs)
        {
            unsigned b = s[p][0];
            if(s[b][0] != b)
                s[p][0] = (this->get(b))[0];
            return s[s[p][0]];
        } else
        {
            this->add(p - s.maxs + 1);
            return s[p];
        }
    }

    void unite(unsigned f, unsigned sc)
    {
        unsigned fr = (this->get(f))[0];
        unsigned sr = (this->get(sc))[0];
        if(fr != sr)
        {
            if(s[fr][1] >= s[sr][1])
            {
                s[fr][2] = min(s[fr][2], s[sr][2]);
                s[fr][3] = max(s[fr][3], s[sr][3]);
                s[fr][4] = s[fr][4] + s[sr][4];
                s[sr][0] = s[fr][0];
                if(s[fr][1] == s[sr][1])
                    s[fr][1]++;
            } else
            {
                s[sr][2] = min(s[fr][2], s[sr][2]);
                s[sr][3] = max(s[fr][3], s[sr][3]);
                s[sr][4] = s[fr][4] + s[sr][4];
                s[fr][0] = s[sr][0];
            }
        }
    }

    bool sameSet(unsigned f, unsigned s)
    {
        return (this->get(f))[0] == (this->get(s))[0];
    }
};

