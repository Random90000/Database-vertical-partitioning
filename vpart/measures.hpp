#include <functional>
#include <vector>

#include "matrix.hpp"

using Measure = std::function<std::vector<int>(Matrix)>;

std::vector<int> voidMeasure(Matrix);
std::vector<int> blockingMeasure(Matrix);
