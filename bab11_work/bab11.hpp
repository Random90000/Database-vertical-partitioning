//
// Created by opot on 10.12.17.
//

#ifndef SOURCE_BAB11_H
#define SOURCE_BAB11_H

#include <vector>
#include <map>

#include "bab.hpp"

struct BabNode {
    BabNode(Matrix* m, float z_low = 0.0);
   ~BabNode();

    void     calculate_void_measures();
    Solution Bab11(const Matrix* m);

    Matrix*                           matrix;
    static float                      threshold;
    float                             z_low;
    std::vector<std::pair<int,int>>*  voids;//<column, void_measure>
    int branchingId = -1;

    bool isBranching();
};

Matrix* duplicate(Matrix* m, int attribute);

std::vector<Matrix*>* cluster_identification(const Matrix* m);

#endif //SOURCE_BAB11_H
