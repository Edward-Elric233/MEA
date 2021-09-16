//
// Created by Administrator on 2021/9/13.
//

#ifndef MEA_RANDOM_H
#define MEA_RANDOM_H

#include <random>
#include <ctime>

class Random {
    std::default_random_engine random;
    std::uniform_int_distribution<int> distribution;
public:
    Random(int _seed = std::time(nullptr))
    : random(_seed) {}
    int getRandom() {
        return distribution(random);
    }
    std::default_random_engine &getRandomEngine () {
        return random;
    }
};


#endif //MEA_RANDOM_H
