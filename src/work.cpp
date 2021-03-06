// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/9/10
// Description: 
#include "work.h"
#include "data.h"
#include <cstdlib>
#include <ctime>
#include <Solution.h>

using namespace std;
using namespace data;

void read(std::istream &is) {
    int N, M, C, K, S, G, D;
    is >> N >> M >> C;
    for (int i = 0; i < M; ++i) {
        machine_vec.emplace_back(new Machine(i));
    }
    for (int i = 0; i < N; ++i) {
        int job_idx = i;
        job_vec.emplace_back(new Job(job_idx));
        is >> K;
        auto &job = job_vec.back();
        //K个工序
        for (int j = 0; j < K; ++j) {
            int order_idx = order_vec.size();
            job->order_vec.push_back(order_idx);
            order_vec.emplace_back(new Order(job_idx, j));
            auto &order = order_vec.back();
            is >> S;
            //S个机器
            for (int k = 0; k < S; ++k) {
                is >> G >> D;
                int occupy_idx = occupy_vec.size();
                occupy_vec.emplace_back(new Occupy(job_idx, order_idx, G, D));
                machine_vec[G]->occupy_vec.push_back(occupy_idx);
                order->occupy_vec.push_back(occupy_idx);
            }
        }
    }
    random = unique_ptr<Random>(new Random(data::seed));
    Solution::init();
}


#include <ostream>
void test_read() {
    cout << job_vec.size() << " " << machine_vec.size() << " 2\n";
    for (auto &job : job_vec) {
        cout << job->order_vec.size();
        for (auto order_idx : job->order_vec) {
            auto &order = order_vec[order_idx];
            cout << " " << order->occupy_vec.size();
            for (int i = 0; i < order->occupy_vec.size(); ++i) {
                int occupy_idx = order->occupy_vec[i];
                cout << " " << occupy_vec[occupy_idx]->machine_idx << " " << occupy_vec[occupy_idx]->time;
            }
        }
        cout << "\n";
    }
}
std::shared_ptr<Solution> MAE() {
    int gen = 0;
    int repeatCount = 0;
    auto S1 = make_shared<Solution>();
    auto S2 = make_shared<Solution>();
    auto Sc = make_shared<Solution>();
    auto Sp = make_shared<Solution>();
    auto S = make_shared<Solution>();
    decltype(S1) S1_, S2_;
    while (repeatCount < data::repeatCount) {
        if (clock() > data::expires) break;
        S1_ = Solution::pathRelink(S1, S2);
        S2_ = Solution::pathRelink(S2, S1);
//        S1_ = S1;
//        S2_ = S2;
        S1 = Solution::tabuSearch(S1_);
        S2 = Solution::tabuSearch(S2_);
        if (*S1 < *Sc) {
            Sc = S1;
        }
        if (*S2 < *Sc) {
            Sc = S2;
        }
        if (*Sc < *S) {
            repeatCount = 0;
            S = Sc;
//            cout << *S << endl;
//            cout << S->getMakeSpan() << endl;
        }
        if (gen == data::p) {
            S1 = Sp;
            Sp = Sc;
            Sc = make_shared<Solution>();
            gen = 0;
        }
        if (Solution::dis(*S1, *S2) < data::eps)  {
            S2 = make_shared<Solution>();
        }
        ++gen;
        ++repeatCount;
    }
    return S;
}
