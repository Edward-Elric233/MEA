// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/9/10
// Description: 
#include "data.h"
#include <ctime>

namespace data {
    std::vector<std::shared_ptr<Job>> job_vec;
    std::vector<std::shared_ptr<Order>> order_vec;
    std::vector<std::shared_ptr<Machine>> machine_vec;
    std::vector<std::shared_ptr<Occupy>> occupy_vec;
    std::unique_ptr<Random> random;
    int seed;
    int time;
    int p = 20;
    double alpha = 0.4;
    double beta = 0.6;
    int gamma = 5;
    int eps = 3;
    int pathLinkIterCount = 10;
    int repeatCount = p * 2;
    clock_t expires;
}