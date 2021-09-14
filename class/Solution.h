//
// Created by Administrator on 2021/9/13.
//

#ifndef MEA_SOLUTION_H
#define MEA_SOLUTION_H

#include "data.h"
#include "Depend.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>


/*!
 * 在读入数据后才能对该类进行实例化
 * 在使用前应该运行init函数进行初始化
 */
class Solution {
    int orderNum, machineNum;
    std::vector<std::vector<int>> schedule;
    Depend depend;
    int makeSpan;
    static std::unique_ptr<Depend> sDepend;

public:
    Solution(int _orderNum = data::order_vec.size(), int _machineNum = data::machine_vec.size())
    : orderNum(_orderNum), machineNum(_machineNum)
    , schedule(_machineNum)
    , depend()
    //TODO:makeSpan注意要在Solution被修改后初始化为-1，否则不会调用getMakeSpan函数
    , makeSpan(-1) {
//        getSolution();
    }

    void getRandomSolution();
    void getSolution();
    bool isFeasible();
    int getMakeSpan();

    static void init();
    static int dis(const Solution &lhs, const Solution &rhs, int orderIdx);

    friend std::istream &operator >> (std::istream &is, Solution &solution);
    friend std::ostream &operator << (std::ostream &os, const Solution &solution);
};

std::istream &operator >> (std::istream &is, Solution &solution);
std::ostream &operator << (std::ostream &os, const Solution &solution);

#endif //MEA_SOLUTION_H
