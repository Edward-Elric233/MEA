//
// Created by Administrator on 2021/9/13.
//

#include "Solution.h"
#include <iostream>
#include <set>
#include <list>
#include <memory>
#include <climits>

std::unique_ptr<Depend> Solution::sDepend;
std::vector<int> Solution::tabuTable;
int Solution::tabuIter = 0;

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
    tabuTable.resize(data::order_vec.size());
}
void Solution::getRandomSolution() {
    for (auto &m : schedule) m.clear();
    //jobIndex orderIndex;
    std::list<std::pair<int, int>> jobList;
    for (int i = 0; i < data::job_vec.size(); ++i) {
        jobList.push_back({i, 0});
    }
    int machineIdx;
    while (!jobList.empty()) {
        int jobIdx = data::random->getRandom() % jobList.size();
        auto iter = jobList.begin();
        for (int i = 0; i < jobIdx; ++i) {
            ++iter;
        }
        auto &job = data::job_vec[iter->first];
        int orderIdx = job->order_vec[iter->second];
        auto &order = data::order_vec[orderIdx];
        machineIdx = data::random->getRandom() % order->occupy_vec.size();
        machineIdx = data::occupy_vec[order->occupy_vec[machineIdx]]->machine_idx;
        schedule[machineIdx].push_back(orderIdx);
        iter->second = iter->second + 1;
        if (iter->second == job->order_vec.size()) {
            jobList.erase(iter);
        }
    }
    makeSpan = -1;
    isSetBelong = false;
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
//    return true;
    return !depend.isCircle();
}

void Solution::getSolution() {
    do {
        getRandomSolution();
    } while (!isFeasible());
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
        return INT_MAX;
    }
    if (makeSpan == -1) {
        getBelong();
        makeSpan = depend.getMakeSpan(path);
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
        }
    }
    solution.makeSpan = -1;
    solution.isSetBelong = false;
    return is;
}

int Solution::dis(Solution &lhs, Solution &rhs, int orderIdx) {
    lhs.getBelong();
    rhs.getBelong();
    auto &lBelong = lhs.depend.belong[orderIdx];
    auto &rBelong = rhs.depend.belong[orderIdx];
    int lMachine = std::get<0>(lBelong);
    int rMachine = std::get<0>(rBelong);
    if (lMachine != rMachine) {
        int lPos = std::get<1>(lBelong);
        int rPos = std::get<1>(rBelong);
        int dis1 = lPos + rPos;
        int dis2 = lhs.schedule[lMachine].size() - lPos + rhs.schedule[rMachine].size() - rPos;
        return std::min(dis1, dis2);
    } else {
        return std::abs(std::get<1>(lBelong) - std::get<1>(rBelong));
    }
}


int Solution::dis(Solution &lhs, Solution &rhs) {
    lhs.getBelong();
    rhs.getBelong();
    int ret = 0;
    for (int i = 0; i < lhs.orderNum; ++i) {
        ret += dis(lhs, rhs, i);
    }
    return ret;
}

std::shared_ptr<Solution> Solution::pathRelink(std::shared_ptr<Solution> Si, std::shared_ptr<Solution> Sg) {
    std::shared_ptr<Solution> Sc = Si;
    Sg->getBelong();
    auto &belong = Sg->depend.belong;
    std::list<std::shared_ptr<Solution>> N;
    std::vector<std::shared_ptr<Solution>> PathSet;
    int count = 0;
//    for (auto &t : Sc->depend.belong) {
//        std::cout << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
//    }
    while (dis(*Sc, *Sg) > data::alpha * dis(*Si, *Sg) && count++ < data::pathLinkIterCount) {
        if (clock() > data::expires) break;
        for (int i = 0; i < data::order_vec.size(); ++i) {
            auto SList = Sc->getN(i, std::get<0>(belong[i]));
//            std::cout << i << std::endl;
//            for (auto S : SList) {
//                std::cout << *S << std::endl;
//            }
            int dis = INT_MAX;
            std::shared_ptr<Solution> Smin;
            //如果没有Solution满足条件，则不要向N添加，继续操作下一个工序
            if (SList.empty()) continue;
            for (auto &S : SList) {
                int t = Solution::dis(*S, *Sg, i);
                if (t < dis) {
                    dis = t;
                    Smin = S;
                }
            }
            N.push_back(Smin);
        }
        //如果已经找不到新的Solution了，则退出循环
        if (N.empty()) break;
        int dis = Solution::dis(*Sc, *Sg);
        std::vector<std::tuple<int, int, std::shared_ptr<Solution>>> indexTmp;
        for (auto iter = N.begin(); iter != N.end(); ) {
            int disTmp = Solution::dis(**iter, *Sg);
            if (disTmp > dis) {
                N.erase(iter++);
            } else {
                indexTmp.push_back({disTmp, (*iter)->getMakeSpan(), *iter});
                iter++;
            }
        }
        if (N.empty()) break;
        std::vector<std::pair<int, std::shared_ptr<Solution>>> index(indexTmp.size());
        for (int i = 0; i < index.size(); ++i) {
            auto &t = indexTmp[i];
            int dis = std::get<0>(t);
            int obj = std::get<1>(t);
            index[i].second = std::get<2>(t);
            index[i].first = 0;
            for (int j = 0; j < indexTmp.size(); ++j) {
                if (std::get<0>(indexTmp[j]) < dis) ++index[i].first;
                if (std::get<1>(indexTmp[j]) < obj) ++index[i].first;
            }
        }
        std::sort(index.begin(), index.end());
        int k = data::random->getRandom() % (std::min(data::gamma, static_cast<int>(index.size())));
        Sc = index[k].second;
        N.clear();
        if (std::get<0>(indexTmp[k]) < data::beta * dis) {
            PathSet.push_back(Sc);
        }
    }
    int makeSpan = INT_MAX;
    //记得对指针进行初始化，空指针的危害很大
    std::shared_ptr<Solution> Sr = Si;
    for (auto S : PathSet) {
        if (S->getMakeSpan() < makeSpan) {
            makeSpan = S->getMakeSpan();
            Sr = S;
        }
    }
    return Sr;
}

void Solution::getBelong() {
    if (isSetBelong) return;
    isSetBelong = true;
    for (int i = 0; i < machineNum; ++i) {
        auto &m = schedule[i];
        for (int j = 0; j < m.size(); ++j) {
            int orderIdx = m[j];
            auto &belong = depend.belong[orderIdx];
            std::get<0>(belong) = i;
            std::get<1>(belong) = j;
            for (auto occpyIdx : data::order_vec[orderIdx]->occupy_vec) {
                if (data::occupy_vec[occpyIdx]->machine_idx == i) {
                    std::get<2>(belong) = data::occupy_vec[occpyIdx]->time;
                    break;
                }
            }
        }
    }
}

bool Solution::setMachineOrder(int orderIdx, int mTarget, int mOrder) {
    getBelong();
    auto &belong = depend.belong[orderIdx];
    int mSource = std::get<0>(belong);
    int originMOrder = std::get<1>(belong);
    if (mSource != mTarget) {
        auto &m2 = schedule[mTarget];
        if (mOrder <0 || mOrder > m2.size()) return false;
        auto &m1 = schedule[mSource];
        for (int i = originMOrder; i < m1.size() - 1; ++i) {
            m1[i] = m1[i + 1];
        }
        m1.pop_back();
        m2.push_back(0);
        for (int i = m2.size() - 1; i > mOrder; --i) {
            m2[i] = m2[i - 1];
        }
        m2[mOrder] = orderIdx;
    } else {
        if (originMOrder == mOrder) return false;
        auto &m = schedule[mSource];
        if (originMOrder < mOrder) {
            for (int i = originMOrder; i < mOrder; ++i) {
                m[i] = m[i + 1];
            }
        } else {
            for (int i = originMOrder; i > mOrder; --i) {
                m[i] = m[i - 1];
            }
        }
        m[mOrder] = orderIdx;
    }
    makeSpan = -1;
    isSetBelong = false;
    return isFeasible();
}

std::vector<std::shared_ptr<Solution>> Solution::getN(int orderIdx, int targetM) {
    getBelong();
    std::vector<std::shared_ptr<Solution>> ret;
    int sourceM = std::get<0>(depend.belong[orderIdx]);
    auto &m = schedule[targetM];
    for (int i = 0; i < m.size(); ++i) {
        std::shared_ptr<Solution> S = std::make_shared<Solution>(*this);
        if (S->setMachineOrder(orderIdx, targetM, i)) {
            ret.push_back(S);
        }
    }
    //如果不在一台机器上是可以放到尾后位置的
    if (sourceM != targetM) {
        std::shared_ptr<Solution> S = std::make_shared<Solution>(*this);
        if (S->setMachineOrder(orderIdx, targetM, m.size())) {
            ret.push_back(S);
        }
    }
    return std::move(ret);
}

std::shared_ptr<Solution> Solution::tabuSearch(std::shared_ptr<Solution> S) {
    std::vector<int> vec;
    S->getMakeSpan();
    std::pair<std::shared_ptr<Solution>, std::shared_ptr<Solution>> pairS = {S, S};
    auto &path = S->path;
    for (auto orderIdx : path) {
        if (Solution::tabuTable[orderIdx] > Solution::tabuIter) continue;
        int tabuLen = S->path.size() + data::order_vec[orderIdx]->occupy_vec.size();
        Solution::tabuTable[orderIdx] += tabuLen;
        auto &occupy = data::order_vec[orderIdx]->occupy_vec;
        for (auto machineIdx : occupy) {
            machineIdx = data::occupy_vec[machineIdx]->machine_idx;
            auto sList = S->getN(orderIdx, machineIdx);
            for (auto &pS : sList) {
                if (pS->getMakeSpan() < pairS.first->getMakeSpan()) {
                    pairS.first = pS;
                } else if ( pS->getMakeSpan() < pairS.second->getMakeSpan()) {
                    pairS.second = pS;
                }
            }
        }
        ++Solution::tabuIter;
    }
    bool first = data::random->getRandom() % 2;
    if (first) {
        return pairS.first;
    } else {
        return pairS.second;
    }
}

bool operator < (Solution &lhs, Solution &rhs) {
    return lhs.getMakeSpan() < rhs.getMakeSpan();
}
