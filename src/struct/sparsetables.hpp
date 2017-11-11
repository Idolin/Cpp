#pragma once

//TODO(SPARSE TABLES)
/*
    unsigned *l = new unsigned[n];
    unsigned now = 0, next = 1;
    for(unsigned i = 0;i<n;i++)
    {
        if(i==next)
        {
            now++;
            next=next*2+1;
        }
        l[i] = now;
    }
    unsigned **st = new unsigned*[l[n-1]+1];
    for(unsigned i = 0;i <= l[n-1];i++)
        st[i] = new unsigned[n];
    for(unsigned i = 0;i < n;i++)
    {
        st[0][i] = a;
        a = (a*23+21563)%16714589;
    }
    unsigned k = 1;
    for(unsigned i = 1;i<=l[n-1];i++)
    {
        for(unsigned o = 0;o+k<n;o++)
            st[i][o] = min(st[i-1][o], st[i-1][o+k]);
        for(unsigned o = n - k;o<n;o++)
            st[i][o] = st[i-1][o];
        k*=2;
    }
    for(unsigned i = 1;;i++)
    {
        if(u>v)
        {
            lb = v;
            rb = u;
        }
        else
        {
            lb = u;
            rb = v;
        }
        unsigned j = l[rb-lb];
        ans = min(st[j][lb-1], st[j][rb-(1 << j)]);
*/

template<typename T>
struct st2d
{
    T **mst2d;
    unsigned l, h;
    unsigned char *m;

    inline T merge(T x1, T x2)
    {
        return x1.intersection(x2);
    }

    inline T merge4(T x1, T x2, T x3, T x4)
    {
        return merge(merge(x1, x2), merge(x3, x4));
    }

    st2d(unsigned l, unsigned h) : l(l), h(h)
    {
        unsigned mx = l > h ? l : h;
        m = new unsigned char[mx];
        unsigned char f = 0;
        for(unsigned k = 0, n = 2; k < mx; k++)
        {
            if(k == n)
            {
                f++;
                n *= 2;
            }
            m[k] = f;
        }
        unsigned fl = l * (m[l - 1] + 1);
        unsigned fh = h * (m[h - 1] + 1);
        mst2d = new T *[fl];
        for(unsigned i = 0; i < fl; i++)
            mst2d[i] = new T[fh];
    }

    void init()
    {
        for(unsigned i = 1, lk = 1; i <= m[l - 1]; i++, lk *= 2)
        {
            for(unsigned il = 0; il < l - lk; il++)
                for(unsigned ih = 0; ih < h; ih++)
                    mst2d[l * i + il][ih] = merge(mst2d[l * (i - 1) + il][ih], mst2d[l * (i - 1) + il + lk][ih]);
            for(unsigned il = l - lk; il < l; il++)
                for(unsigned ih = 0; ih < l; ih++)
                    mst2d[l * i + il][ih] = mst2d[l * (i - 1) + il][ih];
        }
        for(unsigned i = 1, hk = 1; i <= m[h - 1]; i++, hk *= 2)
            for(unsigned k = 0; k <= m[l - 1]; k++)
            {
                for(unsigned il = 0; il < l; il++)
                    for(unsigned ih = 0; ih < h - hk; ih++)
                        mst2d[l * k + il][h * i + ih] = merge(mst2d[l * k + il][h * (i - 1) + ih],
                                                              mst2d[l * k + il][h * (i - 1) + ih + hk]);
                for(unsigned il = 0; il < l; il++)
                    for(unsigned ih = h - hk; ih < h; ih++)
                        mst2d[l * k + il][h * i + ih] = mst2d[l * k + il][h * (i - 1) + ih];
            }

    }

    T get(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
    {
        unsigned char xk = m[x2 - x1];
        unsigned char yk = m[y2 - y1];
        return merge4(mst2d[l * xk + x1][h * yk + y1], mst2d[l * xk + x2 + 1 - (1 << xk)][h * yk + y1], \
            mst2d[l * xk + x1][h * yk + y2 + 1 - (1 << yk)],
                      mst2d[l * xk + x2 + 1 - (1 << xk)][h * yk + y2 + 1 - (1 << yk)]);
    }
};
