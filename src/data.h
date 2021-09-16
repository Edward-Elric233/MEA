//
// Created by edward on 2021/9/10.
//

#ifndef FJSP_DATA_H
#define FJSP_DATA_H

#include "Random.h"
#include "Job.h"
#include "Order.h"
#include "Machine.h"
#include "Occupy.h"
#include <vector>
#include <memory>

namespace data {
    extern std::vector<std::shared_ptr<Job>> job_vec;
    extern std::vector<std::shared_ptr<Order>> order_vec;
    extern std::vector<std::shared_ptr<Machine>> machine_vec;
    extern std::vector<std::shared_ptr<Occupy>> occupy_vec;
    extern std::unique_ptr<Random> random;
    extern int seed;
    extern int time;
    extern int p;
    extern double alpha;
    extern double beta;
    extern int gamma;
    extern int eps;
    extern int pathLinkIterCount;
    extern int repeatCount;
    extern clock_t expires;
}

#endif //FJSP_DATA_H
