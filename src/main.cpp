#include "data.h"
#include "work.h"
#include "Solution.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    data::time = atoi(argv[1]);
    data::seed = atoi(argv[2]);
    ifstream is("../data/data0.txt");
    read(is);
    Solution s;
    s.getSolution();
    cout << s.getMakeSpan() << endl;
    cin >> s;
    cout << s.getMakeSpan() << endl;
    return 0;
}
