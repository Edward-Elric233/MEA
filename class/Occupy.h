//
// Created by edward on 2021/9/10.
//

#ifndef FJSP_OCCUPY_H
#define FJSP_OCCUPY_H


class Occupy {
public:
    int job_idx;
    int order_idx;
    int machine_idx;
    int time;
    Occupy(int _job_idx, int _order_idx, int _machine_idx, int _time)
    : job_idx(_job_idx)
    , order_idx(_order_idx)
    , machine_idx(_machine_idx)
    , time(_time) {}
};


#endif //FJSP_OCCUPY_H
