#pragma once

#include "../template/displaymethods.hpp"
#include "persistancestack.hpp"

template<typename T>
struct stack
{
    T *m;
    unsigned minsize, size, resizekf, pointer;

    stack(unsigned size = 20, unsigned minsize = 20, unsigned resizekf = 3) : minsize(minsize), resizekf(resizekf),
                                                                              pointer(0)
    {
        this->size = minsize < size ? size : minsize;
        m = new T[this->size];
    }

    stack(stack const &f) : minsize(f.minsize), size(f.size),
                            resizekf(f.resizekf), pointer(f.pointer)
    {
        m = new T[size];
        for(unsigned i = 0; i < size; i++)
            m[i] = f.m[i];
    }

    ~stack()
    {
        delete[] m;
    }

    void clear()
    {
        delete[] m;
        size = minsize;
        m = new T[size];
        pointer = 0;
    }

    void resizem()
    {
        unsigned newsize;
        if(resizekf > 1)
            newsize = (size + (size == 0)) * resizekf;
        else
            newsize = ((size + (size < 2)) * 3) / 2;
        T *m2 = new T[newsize];
        for(unsigned t = 0; t < size; t++)
            m2[t] = m[t];
        delete[] m;
        m = m2;
        size = newsize;
    }

    void resized()
    {
        size /= resizekf;
        T *m2 = new T[size];
        for(unsigned t = 0; t < size; t++)
            m2[t] = m[t];
        delete[] m;
        m = m2;
    }

    T peek()
    {
        if(pointer > 0)
            return m[pointer - 1];
        else
            throw 11;
    }

    T get(unsigned p)
    {
        if(p < pointer)
            return m[p];
        else
            throw 11;
    }

    T pop()
    {
        if(pointer > 0)
        {
            if(resizekf > 1 and (unsigned) pointer < size / (2 * resizekf) and size / resizekf >= minsize)
                this->resized();
            T x = m[--pointer];
            return x;
        } else
            throw 11;
    }

    void push(T x)
    {
        if((unsigned) pointer == size)
            this->resizem();
        m[pointer++] = x;
    }

    void display(const char *del = "->")
    {
        if(pointer > 0)
        {
            _tdisplay(m[0]);
            for(unsigned i = 1; i < pointer; i++)
            {
                fputs(del, stdout);
                _tshow(m[i]);
            }
            putchar('\n');
        } else
            puts("Stack is empty");
    }
};

template<typename T>
struct stackm
{
    T *m, min;
    unsigned minsize, size, resizekf;
    int pointer;

    stackm(unsigned size = 20, unsigned minsize = 20, unsigned resizekf = 3)
    {
        this->size = minsize < size ? size : minsize;
        m = new T[this->size];
        this->minsize = minsize;
        this->resizekf = resizekf;
        pointer = -1;
    }

    stackm(stackm const &f) : min(f.min), minsize(f.minsize), size(f.size),
                              resizekf(f.resizekf), pointer(f.pointer)
    {
        m = new T[size];
        for(unsigned i = 0; i < size; i++)
            m[i] = f.m[i];
    }

    ~stackm()
    {
        delete[] m;
    }

    void resizem()
    {
        unsigned newsize;
        if(resizekf > 1)
            newsize = (size + (size == 0)) * resizekf;
        else
            newsize = ((size + (size < 2)) * 3) / 2;
        T *m2 = new T[newsize];
        for(unsigned t = 0; t < size; t++)
            m2[t] = m[t];
        delete[] m;
        m = m2;
        size = newsize;
    }

    void resized()
    {
        size /= resizekf;
        T *m2 = new T[size];
        for(unsigned t = 0; t < size; t++)
            m2[t] = m[t];
        delete[] m;
        m = m2;
    }

    T pop()
    {
        if(pointer >= 0)
        {
            if(resizekf > 1 and (unsigned) pointer < size / (2 * resizekf) and size / resizekf >= minsize)
                this->resized();
            T x = m[pointer--];
            if(x == min and pointer >= 0)
                min = m[pointer--];
            return x;
        } else
            return -1;
    }

    void push(T x)
    {
        if(pointer >= 0 and min >= x)
        {
            if((unsigned) ++pointer + 1 >= size)
                this->resizem();
            m[pointer++] = min;
            min = x;
        } else
        {
            if((unsigned) ++pointer == size)
                this->resizem();
            if(pointer == 0)
                min = x;
        }
        m[pointer] = x;
    }

    void display(const char *del = "->", bool cmin = true)
    {
        if(pointer >= 0)
        {
            T mn = m[0];
            int i = 0;
            while(i < pointer)
            {
                _tshow(m[i]);
                fputs(del, stdout);
                if(m[i + 1] == mn)
                {
                    i++;
                    mn = m[++i];
                } else
                    i++;
            }
            _tdisplay(m[pointer]);
            putchar('\n');
            if(cmin)
            {
                fputs("Min:", stdout);
                _tshow(min);
                putchar('\n');
            }
        } else
            puts("Stack is empty");
    }
};
