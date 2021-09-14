//
// Created by edward on 2021/9/10.
//

#ifndef FJSP_ORDER_H
#define FJSP_ORDER_H

#include <vector>

class Order {
public:
    int job_idx;
    int idx;
    std::vector<int> occupy_vec;
    Order(int _job_idx, int _idx)
    : job_idx(_job_idx)
    , idx(_idx) {}
};


#endif //FJSP_ORDER_H
