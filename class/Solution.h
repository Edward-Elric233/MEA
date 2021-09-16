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
    bool isSetBelong;
    int makeSpan;
    static std::unique_ptr<Depend> sDepend;
    static std::vector<int> tabuTable;
    static int tabuIter;

public:
    std::vector<int> path;
    Solution(int _orderNum = data::order_vec.size(), int _machineNum = data::machine_vec.size())
    : orderNum(_orderNum), machineNum(_machineNum)
    , schedule(_machineNum)
    , depend()
    //TODO:makeSpan注意要在Solution被修改后初始化为-1，否则不会调用getMakeSpan函数
    , makeSpan(-1), isSetBelong(false) {
        getSolution();
    }

    void getRandomSolution();
    void getSolution();
    bool isFeasible();
    void getBelong();
    int getMakeSpan();
    bool setMachineOrder(int orderIdx, int mTarget, int mOrder);
    std::vector<std::shared_ptr<Solution>> getN(int orderIdx, int targetM);

    static void init();
    static int dis(Solution &lhs, Solution &rhs, int orderIdx);
    static int dis(Solution &lhs, Solution &rhs);
    static std::shared_ptr<Solution> pathRelink(std::shared_ptr<Solution> Si, std::shared_ptr<Solution> Sg);
    static std::shared_ptr<Solution> tabuSearch(std::shared_ptr<Solution> S);

    friend std::istream &operator >> (std::istream &is, Solution &solution);
    friend std::ostream &operator << (std::ostream &os, const Solution &solution);
    friend bool operator < (Solution &lhs, Solution &rhs);
};

std::istream &operator >> (std::istream &is, Solution &solution);
std::ostream &operator << (std::ostream &os, const Solution &solution);
bool operator < (Solution &lhs, Solution &rhs);

#endif //MEA_SOLUTION_H
