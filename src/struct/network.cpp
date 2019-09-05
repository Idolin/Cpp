struct network
{
    struct nTransition
    {
        unsigned to, reversed;
        int flow, capacity;

        nTransition(unsigned to, unsigned capacity, unsigned reversed) : to(to), reversed(reversed), flow(0),
                                                                         capacity(capacity)
        {}
    };

    unsigned nLen;
    vect <nTransition> *edge;
    unsigned *rPos, *pNextEdge;
    bool *f;
    unsigned sink;

    network(unsigned n) : nLen(n), edge(new vect<nTransition>[n]), f(new bool[n]), pNextEdge(new unsigned[n])
    {}

    void addEdge(unsigned from, unsigned to, unsigned flow)
    {
        edge[from].push(nTransition(to, flow, edge[to].maxs));
        edge[to].push(nTransition(from, 0, edge[from].maxs - 1));
    }

    bool bfs(unsigned s)
    {
        _fill(f, nLen);
        queue q;
        rPos[s] = 0;
        f[s] = true;
        q.push(s);
        while(q.length)
        {
            unsigned v = q.pop();
            for(unsigned i = 0; i < edge[v].maxs; i++)
            {
                nTransition e = edge[v][i];
                if(!f[e.to] && (e.flow < e.capacity))
                {
                    f[e.to] = true;
                    rPos[e.to] = rPos[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return f[sink];
    }

    unsigned dfs(unsigned s, unsigned maxPossible = UINT_MAX)
    {
        if((s == sink) || (maxPossible == 0))
            return maxPossible;
        for(unsigned i = pNextEdge[s]; i < edge[s].maxs; i++)
        {
            if(rPos[s] + 1 == rPos[edge[s][i].to])
            {
                unsigned flowLeft = edge[s][i].capacity - edge[s][i].flow;
                if(flowLeft)
                {
                    unsigned flow = dfs(edge[s][i].to, min(maxPossible, flowLeft));
                    if(flow)
                    {
                        edge[s][i].flow += flow;
                        edge[edge[s][i].to][edge[s][i].reversed].flow -= flow;
                        return flow;
                    }
                }
            }
            pNextEdge[s]++;
        }
        return 0;
    }

    unsigned maxFlow(unsigned s, unsigned t)
    {
        sink = t;
        unsigned flow = 0;
        while(true)
        {
            if(!bfs(s))
                return flow;
            _fill(pNextEdge, nLen);
            while(true)
            {
                unsigned addFlow = dfs(s);
                if(!addFlow)
                    break;
                flow += addFlow;
            }
        }
    }
};