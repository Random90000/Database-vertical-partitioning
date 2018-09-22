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
    for (int i = 0; i < R; i++)
    {
        in >> m.row_id[i];
    }
    for (int i = 0; i < C; i++)
    {
        in >> m.col_id[i];
    }
    std::cout << m;
    for (auto t : *cluster_identification(&m))
    {
        std::cout << *t << "\n";
    }
    return 0;
}
