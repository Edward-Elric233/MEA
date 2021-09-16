//
// Created by Administrator on 2021/9/14.
//

#ifndef MEA_DEPEND_H
#define MEA_DEPEND_H

#include "data.h"
#include <vector>

class Depend {
public:
    int V, E;
    std::vector<int> head;
    std::vector<std::pair<int, int>> edges;
    int edgeIdx;
    //下面的都是工具
    std::vector<bool> isVis;
    std::vector<bool> isInStack;
    std::vector<int> degree;
    std::vector<std::vector<int>> record;
    bool isCircleUtil(int u);
    void getDegreeUtil(int u);
public:
    //所属机器、在机器上的顺序、需要用时
    std::vector<std::tuple<int, int, int>> belong;
    Depend(int _V = data::order_vec.size(),
           int _E = data::order_vec.size() * 2 - data::job_vec.size() - data::machine_vec.size())
    : V(_V), E(_E)
    , head(_V, -1)
    , edges(_E)
    , edgeIdx(0)
    , degree(_V)
    , belong(_V)
    , isVis(_V)
    , record(_V)
    , isInStack(_V) {}
    Depend(const Depend& rhs): Depend() {
        for (int i = 0; i < V; ++i) {
            head[i] = rhs.head[i];
            belong[i] = rhs.belong[i];
        }
        for (int i = 0; i < E; ++i) {
            edges[i] = rhs.edges[i];
        }
        edgeIdx = rhs.edgeIdx;
    }
    Depend &operator = (const Depend &rhs) {
        for (int i = 0; i < V; ++i) {
            head[i] = rhs.head[i];
        }
        for (int i = 0; i < E; ++i) {
            edges[i] = rhs.edges[i];
        }
        edgeIdx = rhs.edgeIdx;
        return *this;
    }
    void addEdge(int u, int v) {
        edges[edgeIdx].first = v;
        edges[edgeIdx].second = head[u];
        head[u] = edgeIdx++;
    }
    bool isCircle();
    void getDegree();
    int getMakeSpan(std::vector<int> &path);
};


#endif //MEA_DEPEND_H
