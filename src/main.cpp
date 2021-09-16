#include "data.h"
#include "work.h"
#include "Solution.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iterator>
#include <ctime>
#include <queue>

using namespace std;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
//    clock_t start = clock();
    data::time = atoi(argv[1]);
    data::seed = atoi(argv[2]);
    data::expires = clock() + (data::time - 5) * CLOCKS_PER_SEC;
//    ifstream is("../data/fjsp.brandimarte.Mk01.m6j10c3.txt");
    read(std::cin);
    shared_ptr<Solution> ans = make_shared<Solution>();
    for (int i = 0; i < 10; ++i) {
        auto S = MAE();
        if (S->getMakeSpan() < ans->getMakeSpan()) {
            ans = S;
        }
        if (clock() > data::expires) break;
    }
    cout << *ans;
//    cout << (static_cast<double>(clock()) - start) / CLOCKS_PER_SEC << "s\n";
    /*
    Solution S;
    cout << S << endl;
    cout << S.getMakeSpan() << endl;
     */
    /*
    auto S = make_shared<Solution>();
    string line;
    while(getline(cin, line)) {
        S = Solution::tabuSearch(S);
        cout << *S << endl;
        cout << S->getMakeSpan() << endl;
    }
     */
    return 0;
}
