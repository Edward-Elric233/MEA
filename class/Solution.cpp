//
// Created by Administrator on 2021/9/13.
//

#include "Solution.h"
#include <iostream>
#include <set>

std::unique_ptr<Depend> Solution::sDepend;

void Solution::init() {
    sDepend = std::unique_ptr<Depend>(new Depend);
//    std::cout << sDepend->V << " " << sDepend->E << std::endl;
//    std::cout << data::order_vec.size() << " " << data::order_vec.size() * 2 - data::job_vec.size() - data::machine_vec.size() << std::endl;

    for (auto &job : data::job_vec) {
        auto &order = job->order_vec;
        for (int i = 1; i < order.size(); ++i) {
            sDepend->addEdge(order[i - 1], order[i]);
        }
    }
//    std::cout << sDepend->edgeIdx << std::endl;
}
void Solution::getRandomSolution() {
    for (auto &m : schedule) m.clear();
    int machineIdx;
    for (int i = 0; i < orderNum; ++i) {
        auto &belong = depend.belong[i];
        auto &order = data::order_vec[i];
        machineIdx = data::random->getRandomMachine() % order->occupy_vec.size();
        std::get<2>(belong) = data::occupy_vec[order->occupy_vec[machineIdx]]->time;
        machineIdx = data::occupy_vec[order->occupy_vec[machineIdx]]->machine_idx;
        schedule[machineIdx].push_back(i);
        std::get<0>(belong) = machineIdx;
    }
    for (auto &m : schedule) {
        std::shuffle(m.begin(), m.end(), data::random->getRandomEngine());
        for (int i = 0; i < m.size(); ++i) {
            std::get<1>(depend.belong[m[i]]) = i;
        }
    }
}
bool Solution::isFeasible() {
    depend = *sDepend;  //调用拷贝赋值
//    std::cout << depend.edgeIdx << std::endl;
    for (auto &m : schedule) {
        for (int i = 1; i < m.size(); ++i) {
            depend.addEdge(m[i - 1], m[i]);
        }
    }
//    std::cout << depend.edgeIdx << std::endl;
    return !depend.isCircle();
}

void Solution::getSolution() {
    do {
        getRandomSolution();
    } while (!isFeasible());
    makeSpan = -1;
}

std::ostream &operator << (std::ostream &os, const Solution &solution) {
    for (auto &m : solution.schedule) {
        os << m.size();
        for (auto order_idx : m) {
            auto &order = data::order_vec[order_idx];
            os << " " << order->job_idx << " " << order->idx;
        }
        os << "\n";
    }
    return os;
}
int Solution::getMakeSpan() {
    if (!isFeasible()) {
        return -1;
    }
    if (makeSpan == -1) {
        makeSpan = depend.getMakeSpan();
    }
    return makeSpan;
}
std::istream &operator >> (std::istream &is, Solution &solution) {
    int E, J, O;
    auto &schedule = solution.schedule;
    for (auto &m : schedule) m.clear();
    for (int i = 0; i < schedule.size(); ++i) {
        auto &m = schedule[i];
        is >> E;
        int orderIdx;
        for (int j = 0; j < E; ++j) {
            is >> J >> O;
            orderIdx = data::job_vec[J]->order_vec[O];
            m.push_back(orderIdx);
            auto &belong = solution.depend.belong[orderIdx];
            std::get<0>(belong) = i;
            std::get<1>(belong) = j;
            for (auto occupyIdx : data::order_vec[orderIdx]->occupy_vec) {
                if (data::occupy_vec[occupyIdx]->machine_idx = i) {
                    std::get<2>(belong) = data::occupy_vec[occupyIdx]->time;
                    break;
                }
            }
        }
    }
    solution.makeSpan = -1;
    return is;
}

int Solution::dis(const Solution &lhs, const Solution &rhs, int orderIdx) {

}
