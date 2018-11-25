#include <iostream>
#include <fstream>
#include "bab11.hpp"

using namespace std;

int main()
{
    ifstream in("./input_2");
    int n;
    in >> n;
    std::vector<Matrix> clusters;
    for (int t = 0; t < n; t++)
    {
        int R,C;
        in >> R >> C;
        Matrix m(R,C);
        in >> m;
        for (int i = 0; i < R; i++)
        {
            in >> m.row_id[i];
        }
        for (int i = 0; i < C; i++)
        {
            in >> m.col_id[i];
        }
        clusters.push_back(m);
    }
    std::cout << "input\n";
    for (auto k : clusters)
    {
        std::cout << k << "\n";
    }
    std::cout << "output\n";
    for (auto k : merge(clusters).clusters)
    {
        //std::cout << k << "\n";
    }
    /*Solution sol = Bab11(m);
    for (auto k : sol.clusters)
    {
        std::cout << k << "\n";
    }*/
    return 0;
}
