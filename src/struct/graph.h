#pragma once

#include "../other/defdef.h"
#include "../debug/def_debug.h"
#include "../template/arraymethods.hpp"
#include "../tree/heap.hpp"
#include "../container/vector.hpp"

#include <algorithm>
#include <climits>

using std::pair;

//template<bool weighted>
struct graph
{
    template<bool, typename dummy = void>
    struct gt
    {
    };

    template<typename dummy>
    struct gt<true, dummy>
    {
        int x;
    };

    template<bool gX>
    using g = gt<gX>;

    //g<X> a;
    struct gTransition
    {
        unsigned node, reversed;
        int weight;
        bool isBridge, isOnCover;

        gTransition(unsigned node = 0, int weight = 1);

        gTransition(gTransition const &t);
    };

    vect<gTransition> *waysFrom, *waysTo, *waysGo;
    unsigned gLen;
    bool oriented, reversed, cycles, negative;
    unsigned char *_tmp;
    unsigned *_uatmp, _utmp, _utmp2;

    graph(unsigned nodes, bool oriented = true);

    ~graph();

    void addWay(unsigned from, unsigned to, int weight = 1);

    void saveReversed(bool save = true);

    void dfs(unsigned node, void(onEnter)(graph *g, unsigned node) = 0, void(onExit)(graph *g, unsigned node) = 0);

    void dfsA(void(onEnter)(graph *g, unsigned node) = 0, void(onExit)(graph *g, unsigned node) = 0);

    long *dijkstra(unsigned node = 0);

    int *bellman_ford(unsigned node);

    long *shortestWay(unsigned node1);

    unsigned *topsort();

    unsigned *strongcc();

    bool transition(unsigned node1, unsigned node2);

    bool correctPath(unsigned *nodes, unsigned len);

    void _cutDfs(unsigned node, unsigned p);

    unsigned findCutBridges(bool *cutPoints = 0);

    bool _draw(unsigned node, unsigned char color = 1);

    bool isBipartite(bool *bp = 0);

    bool kuhn(unsigned node);

    unsigned max_matching(bool *color, unsigned *matching = nullptr);

    unsigned min_paths_covery();

    void dfsMatching(unsigned node, unsigned *mathcing, bool colored = true);

    bool *min_vertex_covery(bool *color, unsigned *matching);

    unsigned *getEuler(bool cycle);
};

inline void _gpushback(graph *g, unsigned node);

inline void _gdraw(graph *g, unsigned node);
