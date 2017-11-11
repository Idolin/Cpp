#include "graph.h"

graph::gTransition::gTransition(unsigned node, int weight) : node(node), weight(weight), isBridge(false)
{}

graph::gTransition::gTransition(gTransition const &t) : node(t.node), weight(t.weight), isBridge(t.isBridge)
{}

graph::graph(unsigned nodes, bool oriented) : waysFrom(new vect<gTransition>[nodes]), waysTo(0),
                                              gLen(nodes), oriented(oriented), reversed(false), cycles(false),
                                              _tmp(new unsigned char[gLen]), _uatmp(0), _utmp(0)
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
    waysFrom[from].push(gTransition(to, weight));
    if(!oriented)
    {
        waysFrom[from][waysFrom[from].maxs - 1].reversed = waysFrom[to].maxs;
        waysFrom[to].push(gTransition(from, weight));
        waysFrom[to][waysFrom[to].maxs - 1].reversed = waysFrom[from].maxs - 1;
    } else if(reversed)
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
            for(unsigned k = 0; k < waysFrom[i].maxs; k++)
                waysTo[waysFrom[i][k].node].push(gTransition(i, waysFrom[i][k].weight));
    }
    reversed = save;
}

void graph::dfs(unsigned node, void(onEnter)(graph *g, unsigned node), void(onExit)(graph *g, unsigned node))
{
    _tmp[node] = 1;
    if(onEnter)
        onEnter(this, node);
    for(unsigned i = 0; i < waysGo[node].maxs; i++)
        if(_tmp[waysGo[node][i].node] == 1)
            cycles = true;
        else if(!_tmp[waysGo[node][i].node])
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

int *graph::dijkstra(unsigned node)
{
    heap<unsigned> *nextmin = new heap<unsigned>(gLen);
    int *distance = new int[gLen];
    _vfill(distance, gLen, INT_MAX);
    distance[node] = 0;
    nextmin->insert(0, node);
    while(nextmin->heap_size)
    {
        unsigned v = nextmin->get_min();
        for(unsigned i = 0; i < waysGo[v].maxs; i++)
            if(distance[waysGo[v][i].node] > distance[v] + waysGo[v][i].weight)
            {
                distance[waysGo[v][i].node] = distance[v] + waysGo[v][i].weight;
                nextmin->insert(distance[waysGo[v][i].node], waysGo[v][i].node);
            }
    }
    return distance;
}

int *graph::bellman_ford(unsigned node)
{
    int *distance = new int[gLen];
    _vfill(distance, gLen, INT_MAX);
    distance[node] = 0;
    for(;;)
    {
        bool changes = false;
        for(unsigned i = 0; i < gLen; i++)
            for(unsigned k = 0; k < waysGo[i].maxs; k++)
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

int graph::shortestWay(unsigned node1, unsigned node2)
{
    if(!oriented)
        return 0;
    _uatmp = new unsigned[gLen];
    _utmp = gLen;
    dfs(node1, 0, _gpushback);
    if(cycles)
        return 0;
    int *distance = new int[gLen];
    _vfill(distance, gLen, INT_MAX);
    distance[node1] = 0;
    for(unsigned i = _utmp; i < gLen; i++)
        for(unsigned k = 0; k < waysGo[_uatmp[i]].maxs; k++)
            distance[waysGo[_uatmp[i]][k].node] = _min(distance[waysGo[_uatmp[i]][k].node],
                                                       distance[_uatmp[i]] + waysGo[_uatmp[i]][k].weight);
    delete[] _uatmp;
    int ans = distance[node2];
    delete[] distance;
    return ans;
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
    for(unsigned i = 0; i < waysFrom[node1].maxs; i++)
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
    for(unsigned i = 0; i < waysFrom[node].maxs; i++)
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
    for(unsigned i = 0; i < waysFrom[node].maxs; i++)
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
    for(unsigned i = 0; i < waysGo[node].maxs; i++)
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

unsigned graph::max_matching(bool *color)
{
    _uatmp = new unsigned[gLen];
    _vfill(_uatmp, gLen, gLen);
    _utmp = 0;
    for(unsigned i = 0; i < gLen; i++)
        if(color[i])
        {
            if(kuhn(i))
                _utmp++;
            _fill(_tmp, gLen);
        }
    delete[] _uatmp;
    return _utmp;
}

unsigned graph::min_paths_covery()
{
    graph *dv = new graph(gLen * 2, false);
    for(unsigned i = 0; i < gLen; i++)
        for(unsigned k = 0; k < waysGo[i].maxs; k++)
            dv->addWay(i, gLen + waysGo[i][k].node);
    bool *c = new bool[gLen * 2];
    _fill(c, gLen);
    _fill(c + gLen, gLen, true);
    unsigned a = gLen - dv->max_matching(c);
    delete[] c;
    delete dv;
    return a;
}

inline void _gpushback(graph *g, unsigned node)
{
    g->_uatmp[--g->_utmp] = node;
}

inline void _gdraw(graph *g, unsigned node)
{
    g->_uatmp[node] = g->_utmp;
}
