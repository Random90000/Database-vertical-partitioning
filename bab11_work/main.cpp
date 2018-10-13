#include <iostream>
#include <fstream>
#include "bab11.hpp"

using namespace std;

int main()
{
    ifstream in("./input");
    int R,C;
    in >> R >> C;
    Matrix m(R,C);
    in >> m;
    /*for (int i = 0; i < R; i++)
    {
        in >> m.row_id[i];
    }
    for (int i = 0; i < C; i++)
    {
        in >> m.col_id[i];
    }*/
    std::cout << m << "\n";

    Solution sol = Bab11(m);
    for (auto k : sol.clusters)
    {
        std::cout << k << "\n";
    }
    return 0;
}
