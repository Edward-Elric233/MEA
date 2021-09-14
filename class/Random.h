//
// Created by Administrator on 2021/9/13.
//

#ifndef MEA_RANDOM_H
#define MEA_RANDOM_H

#include <random>
#include <ctime>

class Random {
    std::default_random_engine random;
    std::uniform_int_distribution<int> distribution_N;
    std::uniform_int_distribution<int> distribution_M;
public:
    Random(int n, int m, int _seed = std::time(nullptr))
    : random(_seed)
    , distribution_N(0, n - 1)
    , distribution_M(0, m - 1) {}
    int getRandomOrder() {
        return distribution_N(random);
    }
    int getRandomMachine () {
        return distribution_M(random);
    }
    std::default_random_engine &getRandomEngine () {
        return random;
    }
};


#endif //MEA_RANDOM_H
