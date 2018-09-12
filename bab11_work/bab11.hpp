//
// Created by opot on 10.12.17.
//

#ifndef SOURCE_BAB11_H
#define SOURCE_BAB11_H

#include <vector>
#include <set>

#include "bab.hpp"

struct BabNode {
    BabNode(Matrix* m, float z_low = 0.0, std::set<int>* duplicated = new std::set<int>);
    BabNode(const Matrix* m, const float z_low = 0.0, const std::set<int>* duplicated = new std::set<int>);
   ~BabNode();

    void     calculate_void_measures();
    Solution Bab11(const Matrix* m);

    Matrix*                           matrix;
    static float                      threshold;
    float                             z_low;
    std::set<int>*                    duplicated;
    std::vector<std::pair<int, int>>* voids;//<column, void_measure>
    int branchingId = -1;

    bool isBranching();
};

#endif //SOURCE_BAB11_H
