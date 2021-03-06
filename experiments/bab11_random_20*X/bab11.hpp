#ifndef SOURCE_BAB11_H
#define SOURCE_BAB11_H

#include <vector>
#include <map>

#include "bab.hpp"

struct BabNode {
    BabNode();
    BabNode(const Matrix m, float z_low, size_t max_duplicated_id = 0);
    BabNode(BabNode&& other);
    BabNode(const BabNode& other);
    ~BabNode();

    void     calculate_void_measures();
    void     sort_void_measures();
    bool     is_duplicatable(float z_up, size_t id);
    BabNode& operator=(BabNode&& other);

    Matrix                             matrix;
    static float                       threshold;
    size_t                             max_duplicated_id;
    float                              z_low;
    std::vector<std::pair<size_t,int>> voids;//<column, void_measure>
};

Solution Bab11(const Matrix& m);

Matrix compression(const Matrix& m);

Solution merge(std::vector<Matrix> input_clusters);

#endif //SOURCE_BAB11_H
