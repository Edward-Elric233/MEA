//
// Created by Administrator on 2021/9/14.
//

#include "Depend.h"
#include <list>
#include <iostream>

bool Depend::isCircleUtil(int u) {
    isVis[u] = isInStack[u] = true;
    int v;
    for (int edgeIdx = head[u]; edgeIdx != -1; edgeIdx = edges[edgeIdx].second) {
        v = edges[edgeIdx].first;
        if (!isVis[v] && isCircleUtil(v)) return true;
        else if (isInStack[v]) return true;
    }
    isInStack[u] = false;
    return false;
}

bool Depend::isCircle() {
    std::fill(isVis.begin(), isVis.end(), false);
    std::fill(isInStack.begin(), isInStack.end(), false);
    for (int i = 0; i < V; ++i) {
        if (!isVis[i]) {
            if (isCircleUtil(i)) return true;
        }
    }
    return false;
}

void Depend::getDegreeUtil(int u) {
    isVis[u] = true;
    int v;
    for (int edgeIdx = head[u]; edgeIdx != -1; edgeIdx = edges[edgeIdx].second) {
        v = edges[edgeIdx].first;
        ++degree[v];
        if (!isVis[v]) getDegreeUtil(v);
    }
}

void Depend::getDegree() {
    std::fill(isVis.begin(), isVis.end(), false);
    std::fill(degree.begin(), degree.end(), 0);
    for (int i = 0; i < V; ++i) {
        if (!isVis[i]) getDegreeUtil(i);
    }
}

int Depend::getMakeSpan(std::vector<int> &path) {
//    for (int i = 0; i < V; ++i) {
//        auto &order = data::order_vec[i];
//        std::cout << i << " : "  << order->job_idx << " " << order->idx << "\n";
//    }
//    std::cout << std::endl;
    std::list<std::pair<int, int>> orderList;
    for (auto &r : record) r.clear();
    getDegree();
    int ret = 0;
    decltype(orderList.begin()) it, t;
    std::pair<int, int> order;
    for (int i = 0; i < V; ++i) {
        if (degree[i] == 0) {
            orderList.push_back({std::get<2>(belong[i]), i});
            record[i].push_back(i);
        }
    }
    while (!orderList.empty()) {
        order.first = INT_MAX;
        for (it = orderList.begin(); it != orderList.end(); ++it) {
            if (it->first < order.first) {
                order.first = it->first;
                t = it;
            }
        }
        order.second = t->second;
        orderList.erase(t);
        for (it = orderList.begin(); it != orderList.end(); ++it) {
            it->first -= order.first;
        }
        ret += order.first;
        int v;
        for (int edgeIdx = head[order.second]; edgeIdx != -1; edgeIdx = edges[edgeIdx].second) {
            v = edges[edgeIdx].first;
            --degree[v];
            if (degree[v] == 0) {
                orderList.push_back({std::get<2>(belong[v]), v});
                record[v] = record[order.second];
                record[v].push_back(v);
            }
        }
        if (orderList.empty()) {
            path = record[order.second];
        }
    }
    return ret;
}
