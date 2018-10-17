#pragma once

#include "../container/vector.hpp"
#include "rmq.hpp"

template<typename T>
struct heavyLightNodes
{
    unsigned nodesCount, treeCount;
    vect<unsigned> **connectedNodes;
    rmq <T> *_RMQ;
    unsigned *parent, *treeValue, *size, *depth, *startOfHeavyPath;
    T *value, *prermq, neutral;

    heavyLightNodes(unsigned nodesCount, T neutral = 0) : nodesCount(nodesCount), treeCount(0), neutral(neutral)
    {
        connectedNodes = new vect<unsigned> *[nodesCount];
        for(unsigned i = 0; i < nodesCount; i++)
            connectedNodes[i] = new vect<unsigned>(1);
        parent = new unsigned[nodesCount];
        value = new T[nodesCount];
        treeValue = new unsigned[nodesCount];
        size = new unsigned[nodesCount];
        depth = new unsigned[nodesCount];
        startOfHeavyPath = new unsigned[nodesCount];
        prermq = new T[nodesCount];
        for(unsigned i = 0; i < nodesCount; i++)
        {
            parent[i] = 0;
            treeValue[i] = -1;
            value[i] = neutral;
            size[i] = 0;
            depth[i] = 0;
            startOfHeavyPath[i] = 0;
        }
    }

    ~heavyLightNodes()
    {
        delete _RMQ;
        delete[] parent;
        delete[] value;
        delete[] treeValue;
        delete[] depth;
        delete[] startOfHeavyPath;
    }

    T func(T x1, T x2)
    {
        return x1 < x2 ? x2 : x1;
    }

    void dfs(unsigned node, unsigned depthNode = 0)
    {
        depth[node] = depthNode++;
        unsigned sizeOfNode = 1, maxSize = 0, maxSizeNode = 0;
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if(parent[node] != connectedNodes[node]->m[i])
            {
                parent[connectedNodes[node]->m[i]] = node;
                dfs(connectedNodes[node]->m[i], depthNode);
                sizeOfNode += size[connectedNodes[node]->m[i]];
                if(size[connectedNodes[node]->m[i]] > maxSize)
                {
                    maxSize = size[connectedNodes[node]->m[i]];
                    maxSizeNode = connectedNodes[node]->m[i];
                }
            }
        startOfHeavyPath[node] = maxSizeNode;
        size[node] = sizeOfNode;
    }

    void heavyLightDecomposition(unsigned node, unsigned startOfPath = -1)
    {
        if(startOfHeavyPath[node] == 0)
        {
            if(startOfPath != -1)
            {
                prermq[treeCount] = value[node];
                treeValue[node] = treeCount++;
                startOfHeavyPath[node] = startOfPath;
            } else
                treeValue[node] = -1;
            return;
        }
        if(startOfPath == -1)
        {
            if(startOfHeavyPath[startOfHeavyPath[node]] == 0)
            {
                treeValue[startOfHeavyPath[node]] = -1;
                treeValue[node] = -1;
                return;
            } else
                startOfPath = node;
        }
        prermq[treeCount] = value[node];
        treeValue[node] = treeCount++;
        heavyLightDecomposition(startOfHeavyPath[node], startOfPath);
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if((connectedNodes[node]->m[i] != startOfHeavyPath[node]) and
               (connectedNodes[node]->m[i] != parent[node]))
                heavyLightDecomposition(connectedNodes[node]->m[i]);
        startOfHeavyPath[node] = startOfPath;
    }

    void init()
    {
        dfs(0);
        delete[] size;
        heavyLightDecomposition(0);
        for(unsigned i = 0; i < nodesCount; i++)
            delete connectedNodes[i];
        delete[] connectedNodes;
        _RMQ = new rmq<T>(treeCount, neutral, false);
        for(unsigned i = 0; i < treeCount; i++)
            _RMQ->m[i] = prermq[i];
        delete[] prermq;
        _RMQ->init();
    }

    void setValue(unsigned node, T newValue)
    {
        value[node] += newValue;
        if(treeValue[node] != -1)
            _RMQ->set(treeValue[node], value[node]);
    }

    T getFunc(unsigned node1, unsigned node2)
    {
        T ret = value[node1];
        while(node1 != node2)
        {
            if(depth[node1] < depth[node2])
            {
                unsigned tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            if(treeValue[node1] == -1)
            {
                ret = func(ret, value[node1]);
                node1 = parent[node1];
            } else if(treeValue[node2] == -1)
            {
                ret = func(ret, value[node2]);
                node2 = parent[node2];
            } else if(startOfHeavyPath[node1] == startOfHeavyPath[node2])
                return func(ret, _RMQ->get(treeValue[node2], treeValue[node1]));
            else if(depth[startOfHeavyPath[node1]] >= depth[startOfHeavyPath[node2]])
            {
                ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node1]], treeValue[node1]));
                node1 = parent[startOfHeavyPath[node1]];
            } else
            {
                ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node2]], treeValue[node2]));
                node2 = parent[startOfHeavyPath[node2]];
            }
        }
        return func(ret, value[node1]);
    }
};

template<typename T>
struct heavyLightNodesPath
{
    unsigned nodesCount, treeCount;
    vect<unsigned> **connectedNodes;
    rmqrs <T> *_RMQ;
    unsigned *parent, *treeValue, *size, *depth, *startOfHeavyPath;
    T *value, *prermq, neutral;

    heavyLightNodesPath(unsigned nodesCount, T neutral = 0) : nodesCount(nodesCount), treeCount(0), neutral(neutral)
    {
        connectedNodes = new vect<unsigned> *[nodesCount];
        for(unsigned i = 0; i < nodesCount; i++)
            connectedNodes[i] = new vect<unsigned>(1);
        parent = new unsigned[nodesCount];
        value = new T[nodesCount];
        treeValue = new unsigned[nodesCount];
        size = new unsigned[nodesCount];
        depth = new unsigned[nodesCount];
        startOfHeavyPath = new unsigned[nodesCount];
        prermq = new T[nodesCount];
        for(unsigned i = 0; i < nodesCount; i++)
        {
            parent[i] = 0;
            treeValue[i] = -1;
            value[i] = neutral;
            size[i] = 0;
            depth[i] = 0;
            startOfHeavyPath[i] = 0;
        }
    }

    ~heavyLightNodesPath()
    {
        delete _RMQ;
        delete[] parent;
        delete[] value;
        delete[] treeValue;
        delete[] depth;
        delete[] startOfHeavyPath;
    }

    T func(T x1, T x2)
    {
        return x1 + x2;
    }

    void dfs(unsigned node, unsigned depthNode = 0)
    {
        depth[node] = depthNode++;
        unsigned sizeOfNode = 1, maxSize = 0, maxSizeNode = 0;
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if(parent[node] != connectedNodes[node]->m[i])
            {
                parent[connectedNodes[node]->m[i]] = node;
                dfs(connectedNodes[node]->m[i], depthNode);
                sizeOfNode += size[connectedNodes[node]->m[i]];
                if(size[connectedNodes[node]->m[i]] > maxSize)
                {
                    maxSize = size[connectedNodes[node]->m[i]];
                    maxSizeNode = connectedNodes[node]->m[i];
                }
            }
        startOfHeavyPath[node] = maxSizeNode;
        size[node] = sizeOfNode;
    }

    void heavyLightDecomposition(unsigned node, unsigned startOfPath = -1)
    {
        if(startOfHeavyPath[node] == 0)
        {
            if(startOfPath != -1)
            {
                prermq[treeCount] = value[node];
                treeValue[node] = treeCount++;
                startOfHeavyPath[node] = startOfPath;
            } else
                treeValue[node] = -1;
            return;
        }
        if(startOfPath == -1)
        {
            if(startOfHeavyPath[startOfHeavyPath[node]] == 0)
            {
                treeValue[startOfHeavyPath[node]] = -1;
                treeValue[node] = -1;
                return;
            } else
                startOfPath = node;
        }
        prermq[treeCount] = value[node];
        treeValue[node] = treeCount++;
        heavyLightDecomposition(startOfHeavyPath[node], startOfPath);
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if((connectedNodes[node]->m[i] != startOfHeavyPath[node]) and
               (connectedNodes[node]->m[i] != parent[node]))
                heavyLightDecomposition(connectedNodes[node]->m[i]);
        startOfHeavyPath[node] = startOfPath;
    }

    void init()
    {
        dfs(0);
        delete[] size;
        heavyLightDecomposition(0);
        for(unsigned i = 0; i < nodesCount; i++)
            delete connectedNodes[i];
        delete[] connectedNodes;
        _RMQ = new rmqrs<T>(treeCount, neutral, neutral, false);
        for(unsigned i = 0; i < treeCount; i++)
            _RMQ->m[i] = prermq[i];
        delete[] prermq;
        _RMQ->init();
    }

    void setValue(unsigned node, T newValue)
    {
        value[node] += newValue;
        if(treeValue[node] != -1)
            _RMQ->set(treeValue[node], treeValue[node], value[node]);
    }

    void setValue(unsigned node1, unsigned node2, T newValue)
    {
        while(node1 != node2)
        {
            if(depth[node1] < depth[node2])
            {
                unsigned tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            if(treeValue[node1] == -1)
            {
                value[node1] = func(value[node1], newValue);
                node1 = parent[node1];
            } else if(treeValue[node2] == -1)
            {
                value[node2] = func(value[node2], newValue);
                node2 = parent[node2];
            } else if(startOfHeavyPath[node1] == startOfHeavyPath[node2])
            {
                _RMQ->set(treeValue[node2], treeValue[node1], newValue);
                return;
            } else if(depth[startOfHeavyPath[node1]] >= depth[startOfHeavyPath[node2]])
            {
                _RMQ->set(treeValue[startOfHeavyPath[node1]], treeValue[node1], newValue);
                node1 = parent[startOfHeavyPath[node1]];
            } else
            {
                _RMQ->set(treeValue[startOfHeavyPath[node2]], treeValue[node2], newValue);
                node2 = parent[startOfHeavyPath[node2]];
            }
        }
        if(treeValue[node1] != -1)
            _RMQ->set(treeValue[node1], treeValue[node1], newValue);
        else
            value[node1] = func(value[node1], newValue);
    }

    T getFunc(unsigned node)
    {
        if(treeValue[node] != -1)
            return _RMQ->get(treeValue[node], treeValue[node]);
        else
            return value[node];
    }

    T getFunc(unsigned node1, unsigned node2)
    {
        T ret = value[node1];
        while(node1 != node2)
        {
            if(depth[node1] < depth[node2])
            {
                unsigned tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            if(treeValue[node1] == -1)
            {
                ret = func(ret, value[node1]);
                node1 = parent[node1];
            } else if(treeValue[node2] == -1)
            {
                ret = func(ret, value[node2]);
                node2 = parent[node2];
            } else if(startOfHeavyPath[node1] == startOfHeavyPath[node2])
                return func(ret, _RMQ->get(treeValue[node2], treeValue[node1]));
            else if(depth[startOfHeavyPath[node1]] >= depth[startOfHeavyPath[node2]])
            {
                ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node1]], treeValue[node1]));
                node1 = parent[startOfHeavyPath[node1]];
            } else
            {
                ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node2]], treeValue[node2]));
                node2 = parent[startOfHeavyPath[node2]];
            }
        }
        return func(ret, value[node1]);
    }
};

template<typename T>
struct heavyLightEdges
{
    unsigned nodesCount, treeCount;
    vect<unsigned> **connectedNodes;
    rmq <T> *_RMQ;
    unsigned *parent, *treeValue, *size, *depth, *startOfHeavyPath;
    T *value, *prermq, neutral;

    heavyLightEdges(unsigned nodesCount, T neutral = 0) : nodesCount(nodesCount), treeCount(0), neutral(neutral)
    {
        connectedNodes = new vect<unsigned> *[nodesCount];
        for(unsigned i = 0; i < nodesCount; i++)
            connectedNodes[i] = new vect<unsigned>(4);
        parent = new unsigned[nodesCount];
        value = new T[nodesCount];
        treeValue = new unsigned[nodesCount];
        size = new unsigned[nodesCount];
        depth = new unsigned[nodesCount];
        startOfHeavyPath = new unsigned[nodesCount];
        prermq = new T[nodesCount - 1];
        for(unsigned i = 0; i < nodesCount; i++)
        {
            parent[i] = 0;
            treeValue[i] = -1;
            value[i] = neutral;
            size[i] = 0;
            depth[i] = 0;
            startOfHeavyPath[i] = 0;
        }
    }

    ~heavyLightEdges()
    {
        delete _RMQ;
        delete[] parent;
        delete[] value;
        delete[] treeValue;
        delete[] depth;
        delete[] startOfHeavyPath;
    }

    T func(T x1, T x2)
    {
        return x1 < x2 ? x2 : x1;
    }

    void dfs(unsigned node, unsigned depthNode = 0)
    {
        depth[node] = depthNode++;
        unsigned sizeOfNode = 1, maxSize = 0, maxSizeNode = 0;
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if(parent[node] != connectedNodes[node]->m[i])
            {
                parent[connectedNodes[node]->m[i]] = node;
                dfs(connectedNodes[node]->m[i], depthNode);
                sizeOfNode += size[connectedNodes[node]->m[i]];
                if(size[connectedNodes[node]->m[i]] > maxSize)
                {
                    maxSize = size[connectedNodes[node]->m[i]];
                    maxSizeNode = connectedNodes[node]->m[i];
                }
            }
        startOfHeavyPath[node] = maxSizeNode;
        size[node] = sizeOfNode;
    }

    void heavyLightDecomposition(unsigned node, unsigned startOfPath = -1)
    {
        if(startOfHeavyPath[node] == 0)
        {
            if(startOfPath != -1)
            {
                prermq[treeCount] = value[node];
                treeValue[node] = treeCount++;
                startOfHeavyPath[node] = startOfPath;
            } else
                treeValue[node] = -1;
            return;
        }
        if(startOfPath == -1)
        {
            if(startOfHeavyPath[startOfHeavyPath[node]] == 0)
            {
                treeValue[startOfHeavyPath[node]] = -1;
                treeValue[node] = -1;
                return;
            } else
                startOfPath = node;
        } else
        {
            prermq[treeCount] = value[node];
            treeValue[node] = treeCount++;
        }
        heavyLightDecomposition(startOfHeavyPath[node], startOfPath);
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if((connectedNodes[node]->m[i] != startOfHeavyPath[node]) and
               (connectedNodes[node]->m[i] != parent[node]))
                heavyLightDecomposition(connectedNodes[node]->m[i]);
        startOfHeavyPath[node] = startOfPath;
    }

    void init()
    {
        dfs(0);
        delete[] size;
        heavyLightDecomposition(0);
        for(unsigned i = 0; i < nodesCount; i++)
            delete connectedNodes[i];
        delete[] connectedNodes;
        _RMQ = new rmq<T>(treeCount, neutral, false);
        for(unsigned i = 0; i < treeCount; i++)
            _RMQ->m[i] = prermq[i];
        delete[] prermq;
        _RMQ->init();
    }

    void setValue(unsigned node, T newValue)
    {
        value[node] += newValue;
        if(treeValue[node] != -1)
            _RMQ->set(treeValue[node], value[node]);
    }

    T getFunc(unsigned node1, unsigned node2)
    {
        T ret = value[node1];
        while(node1 != node2)
        {
            if(depth[node1] < depth[node2])
            {
                unsigned tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            if(treeValue[node1] == -1)
            {
                ret = func(ret, value[node1]);
                node1 = parent[node1];
            } else if(treeValue[node2] == -1)
            {
                ret = func(ret, value[node2]);
                node2 = parent[node2];
            } else if(startOfHeavyPath[node1] == startOfHeavyPath[node2])
                return func(ret, _RMQ->get(treeValue[node2], treeValue[node1]));
            else if(depth[startOfHeavyPath[node1]] >= depth[startOfHeavyPath[node2]])
            {
                ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node1]], treeValue[node1]));
                node1 = parent[startOfHeavyPath[node1]];
            } else
            {
                ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node2]], treeValue[node2]));
                node2 = parent[startOfHeavyPath[node2]];
            }
        }
        return func(ret, value[node1]);
    }
};

template<typename T>
struct heavyLightEdgesPath
{
    unsigned nodesCount, treeCount;
    vect<unsigned> **connectedNodes;
    rmqrs <T> *_RMQ;
    unsigned *parent, *treeValue, *size, *depth, *startOfHeavyPath;
    T *value, *prermq, neutral;

    heavyLightEdgesPath(unsigned nodesCount, T neutral = 0) : nodesCount(nodesCount), treeCount(0), neutral(neutral)
    {
        connectedNodes = new vect<unsigned> *[nodesCount];
        for(unsigned i = 0; i < nodesCount; i++)
            connectedNodes[i] = new vect<unsigned>(1);
        parent = new unsigned[nodesCount];
        value = new T[nodesCount];
        treeValue = new unsigned[nodesCount];
        size = new unsigned[nodesCount];
        depth = new unsigned[nodesCount];
        startOfHeavyPath = new unsigned[nodesCount];
        prermq = new T[nodesCount - 1];
        for(unsigned i = 0; i < nodesCount; i++)
        {
            parent[i] = 0;
            treeValue[i] = -1;
            value[i] = neutral;
            size[i] = 0;
            depth[i] = 0;
        }
    }

    ~heavyLightEdgesPath()
    {
        delete _RMQ;
        delete[] parent;
        delete[] value;
        delete[] treeValue;
        delete[] depth;
        delete[] startOfHeavyPath;
    }

    T func(T x1, T x2)
    {
        return x1 < x2 ? x1 : x2;
    }

    void dfs(unsigned node, unsigned depthNode = 0)
    {
        depth[node] = depthNode++;
        unsigned sizeOfNode = 1, maxSize = 0, maxSizeNode = 0;
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if(parent[node] != connectedNodes[node]->m[i])
            {
                parent[connectedNodes[node]->m[i]] = node;
                dfs(connectedNodes[node]->m[i], depthNode);
                sizeOfNode += size[connectedNodes[node]->m[i]];
                if(size[connectedNodes[node]->m[i]] > maxSize)
                {
                    maxSize = size[connectedNodes[node]->m[i]];
                    maxSizeNode = connectedNodes[node]->m[i];
                }
            }
        startOfHeavyPath[node] = maxSizeNode;
        size[node] = sizeOfNode;
    }

    void heavyLightDecomposition(unsigned node, unsigned startOfPath = -1)
    {
        if(startOfHeavyPath[node] == 0)
        {
            if(startOfPath != -1)
            {
                prermq[treeCount] = value[node];
                treeValue[node] = treeCount++;
                startOfHeavyPath[node] = startOfPath;
            } else
            {
                treeValue[node] = -1;
                startOfHeavyPath[node] = -1;
            }
            return;
        }
        if(startOfPath == -1)
        {
            if(startOfHeavyPath[startOfHeavyPath[node]] == 0)
            {
                treeValue[startOfHeavyPath[node]] = -1;
                for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
                    if(connectedNodes[node]->m[i] != parent[node])
                        startOfHeavyPath[connectedNodes[node]->m[i]] = -1;
                startOfHeavyPath[node] = -1;
                treeValue[node] = -1;
                return;
            } else
                startOfPath = startOfHeavyPath[node];
        } else
        {
            prermq[treeCount] = value[node];
            treeValue[node] = treeCount++;
        }
        heavyLightDecomposition(startOfHeavyPath[node], startOfPath);
        for(unsigned i = 0; i < connectedNodes[node]->maxs; i++)
            if((connectedNodes[node]->m[i] != startOfHeavyPath[node]) and
               (connectedNodes[node]->m[i] != parent[node]))
                heavyLightDecomposition(connectedNodes[node]->m[i]);
        startOfHeavyPath[node] = startOfPath;
    }

    void init()
    {
        dfs(0);
        delete[] size;
        heavyLightDecomposition(0);
        for(unsigned i = 0; i < nodesCount; i++)
            delete connectedNodes[i];
        delete[] connectedNodes;
        _RMQ = new rmqrs<T>(treeCount, neutral, neutral, false);
        for(unsigned i = 0; i < treeCount; i++)
            _RMQ->m[i] = prermq[i];
        delete[] prermq;
        _RMQ->init();
    }

    void setValue(unsigned node1, unsigned node2, T newValue)
    {
        while(node1 != node2)
        {
            if(depth[node1] < depth[node2])
            {
                unsigned tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            if(treeValue[node1] == -1)
            {
                value[node1] = func(value[node1], newValue);
                node1 = parent[node1];
            } else if(treeValue[node2] == -1)
            {
                value[node2] = func(value[node2], newValue);
                node2 = parent[node2];
            } else if(startOfHeavyPath[node1] == startOfHeavyPath[node2])
            {
                _RMQ->set(treeValue[node2], treeValue[node1], newValue);
                return;
            } else if(depth[startOfHeavyPath[node1]] >= depth[startOfHeavyPath[node2]])
            {
                _RMQ->set(treeValue[startOfHeavyPath[node1]], treeValue[node1], newValue);
                node1 = parent[startOfHeavyPath[node1]];
            } else
            {
                _RMQ->set(treeValue[startOfHeavyPath[node2]], treeValue[node2], newValue);
                node2 = parent[startOfHeavyPath[node2]];
            }
        }
        if(treeValue[node1] != -1)
            _RMQ->set(treeValue[node1], treeValue[node1], newValue);
        else
            value[node1] = func(value[node1], newValue);
    }

    T getFunc(unsigned node1, unsigned node2)
    {
        T ret = neutral;
        while(node1 != node2)
        {
            if(depth[node1] < depth[node2])
            {
                unsigned tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            if(startOfHeavyPath[node1] == -1)
            {
                ret = func(ret, value[node1]);
                node1 = parent[node1];
            } else if(startOfHeavyPath[node2] == -1)
            {
                ret = func(ret, value[node2]);
                node2 = parent[node2];
            } else if(startOfHeavyPath[node1] == startOfHeavyPath[node2])
            {
                if(treeValue[node2] == -1)
                    return func(ret, _RMQ->get(treeValue[startOfHeavyPath[node1]], treeValue[node1]));
                else
                    return func(ret, _RMQ->get(treeValue[node2] + 1, treeValue[node1]));
            } else if(depth[parent[startOfHeavyPath[node1]]] >= depth[parent[startOfHeavyPath[node2]]])
            {
                if(treeValue[node1] == -1)
                {
                    ret = func(ret, value[node1]);
                    node1 = parent[node1];
                } else
                {
                    ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node1]], treeValue[node1]));
                    node1 = parent[startOfHeavyPath[node1]];
                }
            } else
            {
                if(treeValue[node2] == -1)
                {
                    ret = func(ret, value[node2]);
                    node2 = parent[node2];
                } else
                {
                    ret = func(ret, _RMQ->get(treeValue[startOfHeavyPath[node2]], treeValue[node2]));
                    node2 = parent[startOfHeavyPath[node2]];
                }
            }
        }
        return ret;
    }
};
