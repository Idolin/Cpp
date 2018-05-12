#include "graph.h"

graph::gTransition::gTransition(unsigned node, int weight) : node(node), weight(weight), isBridge(false)
{}

graph::gTransition::gTransition(gTransition const &t) : node(t.node), weight(t.weight), isBridge(t.isBridge)
{}

graph::graph(unsigned nodes, bool oriented) : waysFrom(new vect<gTransition>[nodes]), waysTo(0),
                                              gLen(nodes), oriented(oriented), reversed(false), cycles(false),
                                              negative(false), _tmp(new unsigned char[gLen]), _uatmp(0), _utmp(0)
{
    _fill(_tmp, gLen);
    waysGo = waysFrom;
}

graph::~graph()
{
    delete[] waysFrom;
    if(reversed)
        delete[] waysTo;
    delete[] _tmp;
}

void graph::addWay(unsigned from, unsigned to, int weight)
{
    negative |= (weight < 0);
    waysFrom[from].push(gTransition(to, weight));
    if(!oriented)
    {
        waysFrom[from][waysFrom[from].size() - 1].reversed = waysFrom[to].size();
        waysFrom[to].push(gTransition(from, weight));
        waysFrom[to][waysFrom[to].size() - 1].reversed = waysFrom[from].size() - 1;
    }
    elif(reversed)
        waysTo[to].push(gTransition(from, weight));
}

void graph::saveReversed(bool save)
{
    if(!save)
        delete[] waysTo;
    else if(!waysTo)
    {
        waysTo = new vect<gTransition>[gLen];
        for(unsigned i = 0; i < gLen; i++)
            for(unsigned k = 0; k < waysFrom[i].size(); k++)
                waysTo[waysFrom[i][k].node].push(gTransition(i, waysFrom[i][k].weight));
    }
    reversed = save;
}

void graph::dfs(unsigned node, void(onEnter)(graph *g, unsigned node), void(onExit)(graph *g, unsigned node))
{
    _tmp[node] = 1;
    if(onEnter)
        onEnter(this, node);
    for(unsigned i = 0; i < waysGo[node].size(); i++)
        if(_tmp[waysGo[node][i].node] == 1)
            cycles = true;
        elif(!_tmp[waysGo[node][i].node])
            dfs(waysGo[node][i].node, onEnter, onExit);
    if(onExit)
        onExit(this, node);
    _tmp[node] = 2;
}

void graph::dfsA(void(onEnter)(graph *g, unsigned node), void(onExit)(graph *g, unsigned node))
{
    for(unsigned i = 0; i < gLen; i++)
        if(!_tmp[i])
            dfs(i, onEnter, onExit);
    _fill(_tmp, gLen);
}

long *graph::dijkstra(unsigned node)
{
    heap<unsigned, long> *nextmin = new heap<unsigned, long>(gLen);
    long *distance = new long[gLen];
    _vfill(distance, gLen, LONG_MAX);
    distance[node] = 0;
    nextmin->insert(0, node);
    while(nextmin->heap_size)
    {
        unsigned v = nextmin->get_min();
        for(unsigned i = 0; i < waysGo[v].size(); i++)
            if(distance[waysGo[v][i].node] > distance[v] + waysGo[v][i].weight)
            {
                distance[waysGo[v][i].node] = distance[v] + waysGo[v][i].weight;
                nextmin->insert(distance[waysGo[v][i].node], waysGo[v][i].node);
            }
    }
    return distance;
}

//long long *floyd() //no negative cycles
//{
//    for(unsigned i = 0;i < n;i++)
//        for(unsigned k = 0;k < n;k++)
//            for(unsigned j = 0;j < n;j++)
//                smin_(d[k][j], d[k][i] + d[i][j]);
//}

//long *bellman_ford(unsigned node)
//{
//    for(unsigned j = 1;j <= n;j++)
//    {
//        bool changes = false;
//        for(unsigned i = 0; i < n; i++)
//            if(distance[i] < mx)
//                for(unsigned k = 0; k < waysGo[i].maxs; k++)
//                    if((distance[waysGo[i][k].node] > mn) &&
//                       (distance[waysGo[i][k].node] > distance[i] + waysGo[i][k].weight))
//                    {
//                        changes = true;
//                        distance[waysGo[i][k].node] = distance[i] + waysGo[i][k].weight;
//                    }
//        if(!changes)
//            break;
//        if(j == n)
//        {
//            for(unsigned i = 0; i < n; i++)
//                for(unsigned k = 0; k < waysGo[i].maxs; k++)
//                    if((distance[waysGo[i][k].node] != mx) && ((distance[waysGo[i][k].node] <= mn) ||
//                                                               (distance[waysGo[i][k].node] > distance[i] + waysGo[i][k].weight)))
//                        f[waysGo[i][k].node] = true;
//            for(unsigned i = 0; i < n; i++)
//                if(f[i])
//                {
//                    q[m++] = i;
//                    distance[i] = mn;
//                }
//            while(s < m)
//            {
//                unsigned v = q[s++];
//                for(unsigned r = 0; r < waysGo[v].maxs; r++)
//                    if(!f[waysGo[v][r].node])
//                    {
//                        distance[waysGo[v][r].node] = mn;
//                        f[waysGo[v][r].node] = true;
//                        q[m++] = waysGo[v][r].node;
//                    }
//            }
//        }
//    }
//}

int *graph::bellman_ford(unsigned node)
{
    int *distance = new int[gLen];
    _vfill(distance, gLen, INT_MAX);
    distance[node] = 0;
    for(;;)
    {
        bool changes = false;
        for(unsigned i = 0; i < gLen; i++)
            for(unsigned k = 0; k < waysGo[i].size(); k++)
                if((distance[waysGo[i][k].node] == INT_MAX) ||
                   (distance[waysGo[i][k].node] > distance[k] + waysGo[i][k].weight))
                {
                    changes = true;
                    distance[waysGo[i][k].node] = distance[k] + waysGo[i][k].weight;
                }
        if(!changes)
            return distance;
    }
}

long *graph::shortestWay(unsigned node)
{
    _uatmp = new unsigned[gLen];
    _utmp = gLen;
    dfs(node, nullptr, _gpushback);
    if(cycles && negative)
        return nullptr;
    if(!cycles)
    {
        long *distance = new long[gLen];
        _vfill(distance, gLen, std::numeric_limits<long>::max());
        distance[node] = 0;
        for(unsigned i = _utmp; i < gLen; i++)
            for(unsigned k = 0; k < waysGo[_uatmp[i]].size(); k++)
                distance[waysGo[_uatmp[i]][k].node] = _min(distance[waysGo[_uatmp[i]][k].node],
                                                           distance[_uatmp[i]] + waysGo[_uatmp[i]][k].weight);
        delete[] _uatmp;
        return distance;
    } elif(!negative)
        return dijkstra(node);
    else
        return nullptr;
}

unsigned *graph::topsort()
{
    if(!oriented)
        return 0;
    _uatmp = new unsigned[gLen];
    _utmp = gLen;
    dfsA(0, _gpushback);
    return _uatmp;
}

unsigned *graph::strongcc()
{
    _utmp = 0;
    if(oriented)
    {
        unsigned *order = this->topsort();
        _uatmp = new unsigned[gLen];
        this->saveReversed();
        waysGo = waysTo;
        for(unsigned i = 0; i < gLen; i++)
            if(!_tmp[order[i]])
            {
                dfs(order[i], _gdraw);
                _utmp++;
            }
        waysGo = waysFrom;
        delete[] order;
    } else
    {
        _uatmp = new unsigned[gLen];
        for(unsigned i = 0; i < gLen; i++)
            if(!_tmp[i])
            {
                dfs(i, _gdraw);
                _utmp++;
            }
    }
    _fill(_tmp, gLen);
    return _uatmp;
}

bool graph::transition(unsigned node1, unsigned node2)
{
    for(unsigned i = 0; i < waysFrom[node1].size(); i++)
        if(waysFrom[node1][i].node == node2)
            return true;
    return false;
}

bool graph::correctPath(unsigned *nodes, unsigned len)
{
    for(unsigned i = 1; i < len; i++)
        if(!transition(nodes[i - 1], nodes[i]))
            return false;
    return true;
}

void graph::_cutDfs(unsigned node, unsigned p)
{
    _tmp[node] = 1;
    _uatmp[node] = _uatmp[node + gLen] = _utmp++;
    unsigned c = 0;
    for(unsigned i = 0; i < waysFrom[node].size(); i++)
        if(waysFrom[node][i].node != p)
        {
            if(_tmp[waysFrom[node][i].node])
                _uatmp[node + gLen] = _min(_uatmp[node + gLen], _uatmp[waysFrom[node][i].node]);
            else
            {
                _cutDfs(waysFrom[node][i].node, node);
                c++;
                _uatmp[node + gLen] = _min(_uatmp[node + gLen], _uatmp[waysFrom[node][i].node + gLen]);
                if(_uatmp[waysFrom[node][i].node + gLen] >= _uatmp[node])
                {
                    if(_uatmp[waysFrom[node][i].node + gLen] > _uatmp[node])
                    {
                        waysFrom[node][i].isBridge = true;
                        waysFrom[waysFrom[node][i].node][waysFrom[node][i].reversed].isBridge = true;
                        _utmp++;
                    }
                    if(p != (unsigned) -1)
                        _tmp[node] = 2;
                }
            }
        }
    if((p == (unsigned) -1) && (c > 1))
        _tmp[node] = 2;
}

unsigned graph::findCutBridges(bool *cutPoints)
{
    if(oriented)
        return 0;
    _uatmp = new unsigned[gLen * 2];
    _utmp = _utmp2 = 0;
    for(unsigned i = 0; i < gLen; i++)
        if(!_tmp[i])
            _cutDfs(i, (unsigned) -1);
    if(cutPoints)
        for(unsigned i = 0; i < gLen; i++)
            if((cutPoints[i] = (_tmp[i] == 2)))
                _utmp2++;
    _fill(_tmp, gLen);
    delete[] _uatmp;
    return _utmp - gLen;
}

bool graph::_draw(unsigned node, unsigned char color)
{
    _tmp[node] = color;
    for(unsigned i = 0; i < waysFrom[node].size(); i++)
        if(!_tmp[waysFrom[node][i].node])
        {
            if(!_draw(waysFrom[node][i].node, color ^ 3))
                return false;
        } else if(_tmp[waysFrom[node][i].node] == color)
            return false;
    return true;
}

bool graph::isBipartite(bool *bp)
{
    bool ok = true;
    for(unsigned i = 0; i < gLen; i++)
        if(!_tmp[i])
            if(!_draw(i))
            {
                ok = false;
                break;
            }
    if(bp && ok)
        for(unsigned i = 0; i < gLen; i++)
            bp[i] = (_tmp[i] == 1);
    _fill(_tmp, gLen);
    return ok;
}

bool graph::kuhn(unsigned node)
{
    _tmp[node] = 1;
    for(unsigned i = 0; i < waysGo[node].size(); i++)
    {
        unsigned to = waysGo[node][i].node;
        if((_uatmp[to] == gLen) || (!_tmp[_uatmp[to]] && kuhn(_uatmp[to])))
        {
            _uatmp[to] = node;
            return true;
        }
    }
    return false;
}

unsigned graph::max_matching(bool *color, unsigned *matching)
{
    _uatmp = new unsigned[gLen];
    _vfill(_uatmp, gLen, gLen);
    _utmp = 0;
    for(unsigned i = 0; i < gLen; i++)
        if(!color[i])
        {
            if(kuhn(i))
                _utmp++;
            _fill(_tmp, gLen);
        }
    if(matching)
        _copy(matching, gLen, _uatmp);
    delete[] _uatmp;
    return _utmp;
}

unsigned graph::min_paths_covery()
{
    graph *dv = new graph(gLen * 2, false);
    for(unsigned i = 0; i < gLen; i++)
        for(unsigned k = 0; k < waysGo[i].size(); k++)
            dv->addWay(i, gLen + waysGo[i][k].node);
    bool *c = new bool[gLen * 2];
    _fill(c, gLen);
    _fill(c + gLen, gLen, true);
    unsigned a = gLen - dv->max_matching(c);
    delete[] c;
    delete dv;
    return a;
}

void graph::dfsMatching(unsigned node, unsigned *mathcing, bool colored)
{
    _tmp[node] = 1;
    if(colored)
    {
        for(unsigned i = 0; i < waysGo[node].size(); i++)
            if(!_tmp[waysGo[node][i].node] && (mathcing[node] != waysGo[node][i].node))
                dfsMatching(waysGo[node][i].node, mathcing, false);
    } else
    {
        for(unsigned i = 0; i < waysGo[node].size(); i++)
            if(!_tmp[waysGo[node][i].node] && (mathcing[waysGo[node][i].node] == node))
                dfsMatching(waysGo[node][i].node, mathcing, true);
    }
}

bool *graph::min_vertex_covery(bool *color, unsigned *matching)
{
    bool *color_mt;
    unsigned *matching_vc;
    if(!(color))
    {
        color_mt = new bool[gLen];
        if(!isBipartite(color_mt))
            return 0;
    } else
        color_mt = color;
    if(!(matching) || !(color))
    {
        matching_vc = new unsigned[gLen];
        max_matching(color_mt, matching_vc);
    } else
        matching_vc = matching;
    for(unsigned i = 0; i < gLen; i++)
        if(color_mt[i] && (matching_vc[i] == gLen) && (waysGo[i].size() > 0) && !_tmp[i])
            dfsMatching(i, matching_vc);
    bool *color_vc;
    if(color)
        color_vc = new bool[gLen];
    else
        color_vc = color_mt;
    for(unsigned i = 0; i < gLen; i++)
        color_vc[i] = color_mt[i] ^ (_tmp[i]);
    if(!(matching) || !(color))
        delete[] matching_vc;
    _fill(_tmp, gLen);
    return color_vc;
}

unsigned *graph::getEuler(bool cycle)
{
    unsigned edges = 0, oddVertex = 0;
    unsigned char odd = 0;
    unsigned *d = new unsigned[gLen];
    _fill(d, gLen);
    for(unsigned i = 0; i < gLen; i++)
    {
        edges += waysGo[i].size();
        for(unsigned r = 0; r < waysGo[i].size(); r++)
            d[waysGo[i][r].node]++;
    }
    for(unsigned i = 0; i < gLen; i++)
        if(waysGo[i].size() > d[i])
        {
            oddVertex = i;
            odd++;
            break;
        }
    if(!oriented)
        edges >>= 1;
    if(odd && cycle)
        return nullptr;
    if(!cycle && (odd > 2))
        return nullptr;
    _uatmp = new unsigned[edges + 1];
    _uatmp[edges] = oddVertex;
    _utmp = 0;
    _utmp2 = edges;
    unsigned *p = new unsigned[gLen];
    _fill(p, gLen);
    while(_utmp2 <= edges)
    {
        unsigned v = _uatmp[_utmp2];
        bool pop = true;
        while(p[v] < waysGo[v].size())
        {
            if(!waysGo[v][p[v]].isOnCover)
            {
                waysGo[v][p[v]].isOnCover = true;
                if(!oriented)
                    waysGo[waysGo[v][p[v]].node][waysGo[v][p[v]].reversed].isOnCover = true;
                _uatmp[--_utmp2] = waysGo[v][p[v]].node;
                p[v]++;
                pop = false;
                break;
            }
            p[v]++;
        }
        if(pop)
        {
            _uatmp[_utmp++] = v;
            _utmp2++;
        }
    }
    delete[] p;
    if(_utmp <= edges)
    {
        delete[] _uatmp;
        return nullptr;
    }
    return _uatmp;
}

inline void _gpushback(graph *g, unsigned node)
{
    g->_uatmp[--g->_utmp] = node;
}

inline void _gdraw(graph *g, unsigned node)
{
    g->_uatmp[node] = g->_utmp;
}
