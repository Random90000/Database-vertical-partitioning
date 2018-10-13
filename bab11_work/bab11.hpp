//
// Created by opot on 10.12.17.
//

#ifndef SOURCE_BAB11_H
#define SOURCE_BAB11_H

#include <vector>
#include <map>

#include "bab.hpp"

struct BabNode {
    BabNode();
    BabNode(const Matrix m, float z_low = 0.0);
    BabNode(BabNode&& other);
    BabNode(const BabNode& other);
    //BabNode(Matrix&& m, float z_low = 0.0);
    ~BabNode();

    void     calculate_void_measures();
    bool     is_duplicatable(float z_up, std::set<std::set<int>>& branches, size_t id);
    BabNode& operator=(BabNode&& other);

    Matrix                             matrix;
    static float                       threshold;
    float                              z_low;
    std::vector<std::pair<size_t,int>> voids;//<column, void_measure>
    std::set<int>                      duplicated;
};

Solution Bab11(const Matrix& m);

//Matrix* duplicate(Matrix* m, int attribute);

//std::vector<Matrix*>* cluster_identification(const Matrix* m);

#endif //SOURCE_BAB11_H
