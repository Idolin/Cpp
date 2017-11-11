#include <stdio.h>

template<typename T>
struct vectm
{
    T *m;
    int size, maxs, resizekf;
};

template<typename T>
vectm<T> ivectm()
{
    vectm<T> *m = new vectm<T>;
    m->size = 20;
    m->m = new T[20];
    m->resizekf = 3;
    m->maxs = -1;
    return *m;
}

template<typename T>
vectm<T> ivectm(unsigned size)
{
    vectm<T> *m = new vectm<T>;
    m->size = size;
    m->m = new T[size];
    m->resizekf = 3;
    m->maxs = -1;
    return *m;
}

template<typename T>
vectm<T> ivectm(unsigned size, unsigned resizekf)
{
    vectm<T> *m = new vectm<T>;
    m->size = size;
    m->m = new T[size];
    m->resizekf = resizekf;
    m->maxs = -1;
    return *m;
}

template<typename T>
void resizemm(vectm<T> &v, int k)
{
    T *m2 = new T[v.size * k];
    for(int t = 0; t < v.size; t++)
        m2[t] = v.m[t];
    delete[] v.m;
    v.m = m2;
    v.size = v.size * k;
}

template<typename T>
void resizetm(vectm<T> &v, int k)
{
    T *m2 = new T[k];
    int mn = v.size < k ? v.size : k;
    for(int t = 0; t < mn; t++)
        m2[t] = v.m[t];
    delete[] v.m;
    v.m = m2;
    v.size = k;
}

template<typename T>
T getm(vectm<T> &v, int p)
{
    return v.m[p];
}

template<typename T>
void setm(vectm<T> &v, int p, T x)
{
    if(p >= v.size)
    {
        int k = v.resizekf;
        if(k > 1)
        {
            while(p >= v.size * k)
                k *= v.resizekf;
            resizemm(v, k);
        } else
        {
            resizetm(v, p);
        }
    }
    if(p > v.maxs)
        v.maxs = p;
    v.m[p] = x;
}

template<typename T>
struct queuem
{
    int blocksize, len;
    char *head, *tail, *headlast, *taillast;
};

template<typename T>
queuem<T> iqueuem()
{
    queuem<T> *q = new queuem<T>;
    q->blocksize = 300;
    q->head = 0;
    q->tail = 0;
    q->len = 0;
    return *q;
}

template<typename T>
queuem<T> iqueuem(int blocksize)
{
    queuem<T> *q = new queuem<T>;
    q->blocksize = blocksize;
    q->head = 0;
    q->tail = 0;
    q->len = 0;
    return *q;
}

template<typename T>
void pushm(queuem<T> &q, T x)
{
    if(q.tail == 0)
    {
        q.tail = new char[q.blocksize * sizeof(x) + sizeof(&x)];
        q.head = q.tail;
        q.taillast = q.tail + q.blocksize * sizeof(T);
        q.headlast = q.taillast;
    } else if(q.tail == q.taillast)
    {
        *(char **) q.tail = new char[q.blocksize * sizeof(x) + sizeof(&x)];
        q.tail = *(char **) q.tail;
        q.taillast = q.tail + q.blocksize * sizeof(x);
    }
    *(T *) q.tail = x;
    q.tail += sizeof(x);
    q.len++;
}

template<typename T>
T popm(queuem<T> &q)
{
    if(q.head == 0 or q.head == q.tail)
        return -1;
    T *y = (T *) q.head;
    q.head += sizeof(T);
    if(q.head == q.headlast)
    {
        q.headlast -= q.blocksize * sizeof(T);
        q.head = *(char **) q.head;
        delete[] q.headlast;
        q.headlast = q.head + q.blocksize * sizeof(T);
    }
    q.len--;
    return *y;
}

template<typename T>
struct tcomp
{
    tcomp<T> **p1;//childrens
    T p2;//value
};

template<typename T>
tcomp<T> *itcomp()
{
    tcomp<T> *c = new tcomp<T>();
    c->p1 = 0;
    c->p2 = 0;
    return c;
}

template<typename T>
struct trie
{
    tcomp<T> *t;//root
    //bool obj;
};

template<typename T>
trie<T> itrie()
{
    trie<T> *t = new trie<T>;
    t->t = itcomp<T>();
    //t -> obj = false;
    return *t;
}

//template<typename T>
//trie<T> itrie(bool obj)
//{
//    trie<T> *t = new trie<T>;
//    t -> t = itcomp<T>();
//    t -> obj = obj;
//    return *t;
//}

template<typename T>
void tset(trie<T> &t, char *s, T v)
{
    unsigned char c = s[0];
    int i = 0;
    tcomp<T> *tc = t.t;
    while(c != 0)
    {
        if((tc->p1 != 0) && (tc->p1[c] != 0))
            tc = tc->p1[c];
        else
        {
            tcomp<T> *n = itcomp<T>();
            if(tc->p1 == 0)
                tc->p1 = new tcomp<T> *[256];
            tc->p1[c] = n;
            tc = n;
        }
        c = s[++i];
    }
    tc->p2 = v;
}

template<typename T>
bool tget(trie<T> &t, char *s, T &v)
{
    unsigned char c = s[0];
    int i = 0;
    tcomp<T> *tc = t.t;
    while(c != 0)
    {
        if((tc->p1 != 0) && (tc->p1[c] != 0))
            tc = tc->p1[c];
        else
        {
            return false;
        }
        c = s[++i];
    }
    if(tc->p2 != 0)
        v = tc->p2;
    return tc->p2 != 0;
}

template<typename T>
void tdel(trie<T> &t, char *s)
{
    char c = s[0];
    int i = 0;
    tcomp<T> *tc = t.t;
    while(c != 0)
    {
        if(tc->p1 != 0 and tc->p1[c] != 0)
            tc = tc->p1[c];
        else
        {
            return;
        }
        c = s[++i];
    }
    if(tc->p2 != 0)
    {
        if(t.obj)
            delete tc->p2;
        tc->p2 = 0;
    }
}

int toint(vectm<char> *s)
{
    int p = 0;
    int r = 0;
    if(s->m[0] == '-')
        p++;
    while(s->m[p] != 0)
        r = r * 10 + s->m[p++] - '0';
    if(s->m[0] == '-')
        return -r;
    else
        return r;
}

int quack_main()
{
    freopen("quack.in", "r", stdin);
    freopen("quack.out", "w", stdout);
    queuem<int> q = iqueuem<int>();
    trie<long> t = itrie<long>();
    int *r = new int[26];
    char c = 0;
    vectm<char> s = ivectm<char>();
    vectm<char> s2 = ivectm<char>();
    int l = 0;
    bool j = false;
    while(c != 'Q' and c != -1)
    {
        c = getchar();
        //usleep(2000);
        if('0' <= c and c <= '9')
        {
            l = 0;
            setm(s, 0, c);
            while(s.m[l] != '\n' and s.m[l] != -1)
            {
                l++;
                setm(s, l, (char) getchar());
            }
            s.m[l] = 0;
            pushm(q, toint(&s));
        } else if(c == '+')
        {
            pushm(q, (popm(q) + popm(q)) % 65536);
        } else if(c == '-')
        {
            pushm(q, (popm(q) - popm(q) + 65536) % 65536);
        } else if(c == '*')
        {
            pushm(q, (popm(q) * popm(q)) % 65536);
        } else if(c == '/')
        {
            int x = popm(q);
            int y = popm(q);
            if(y == 0)
                pushm(q, 0);
            else
                pushm(q, x / y);
        } else if(c == '%')
        {
            int x = popm(q);
            int y = popm(q);
            if(y == 0)
                pushm(q, 0);
            else
                pushm(q, x % y);
        } else if(c == '>')
        {
            r[getchar() - 'a'] = popm(q);
        } else if(c == '<')
        {
            pushm(q, r[getchar() - 'a']);
        } else if(c == 'P')
        {
            c = getchar();
            if('a' <= c and c <= 'z')
                printf("%i\n", r[c - 'a']);
            else
                printf("%i\n", popm(q));
        } else if(c == 'C')
        {
            c = getchar();
            if('a' <= c and c <= 'z')
                printf("%c", r[c - 'a']);
            else
                printf("%c", popm(q));
        } else if(c == ':')
        {
            l = -1;
            do
            {
                setm(s, ++l, (char) getchar());
            } while(s.m[l] != -1 and s.m[l] != '\n');
            s.m[l] = 0;
            tset(t, s.m, ftell(stdin));
        } else if(c == 'J')
        {
            l = -1;
            do
            {
                setm(s, ++l, (char) getchar());
            } while(s.m[l] != -1 and s.m[l] != '\n');
            s.m[l] = 0;
            j = true;
        } else if(c == 'Z')
        {
            c = getchar();
            l = -1;
            do
            {
                setm(s, ++l, (char) getchar());
            } while(s.m[l] != -1 and s.m[l] != '\n');
            s.m[l] = 0;
            if(r[c - 'a'] == 0)
                j = true;
        } else if(c == 'E')
        {
            c = getchar();
            char c2 = getchar();
            l = -1;
            do
            {
                setm(s, ++l, (char) getchar());
            } while(s.m[l] != -1 and s.m[l] != '\n');
            s.m[l] = 0;
            if(r[c - 'a'] == r[c2 - 'a'])
                j = true;
        }
        if(c == 'G')
        {
            c = getchar();
            char c2 = getchar();
            l = -1;
            do
            {
                setm(s, ++l, (char) getchar());
            } while(s.m[l] != -1 and s.m[l] != '\n');
            s.m[l] = 0;
            if(r[c - 'a'] > r[c2 - 'a'])
                j = true;
        }
        if(j)
        {
            long ft;
            j = tget(t, s.m, ft);
            if(j)
                fseek(stdin, ft, 0);
            else
            {
                while(not(j))
                {
                    while(getchar() != ':');
                    l = -1;
                    do
                    {
                        setm(s2, ++l, (char) getchar());
                    } while(s2.m[l] != -1 and s2.m[l] != '\n');
                    s2.m[l] = 0;
                    tset(t, s2.m, ftell(stdin));
                    l = 0;
                    j = true;
                    while(j)
                        j = ((s.m[l] == s2.m[l]) and (s.m[l] != 0) and (s.m[l++] != 0));
                    if(s.m[l] == s2.m[l] and s.m[l] == 0)
                        j = true;
                }
            }
            j = false;
        }
    }
    return 0;
}
