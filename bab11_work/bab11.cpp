
#include "bab11.hpp"
#include <iostream>
#include <fstream>
#include <string.h>

//#define DEBUG

float BabNode::threshold = 0.7;

BabNode::BabNode()
    :matrix(Matrix(0,0)){}

BabNode::BabNode(const Matrix m, float z_low)
    :matrix(m), z_low(z_low), voids(std::vector<std::pair<size_t,int>>(0)){}

BabNode::BabNode(BabNode&& other)
    :matrix(std::move(other.matrix)), z_low(std::move(other.z_low)),
     voids(std::move(other.voids)), duplicated(std::move(other.duplicated)){}

BabNode& BabNode::operator=(BabNode&& other){
    this->matrix     = std::move(other.matrix);
    this->z_low      = std::move(other.z_low);
    this->voids      = std::move(other.voids);
    this->duplicated = std::move(other.duplicated);
    return *this;
}

/*
BabNode::BabNode(Matrix&& m, float z_low)
    :matrix(m), z_low(z_low), voids(){}
*/
std::vector<int> voidMeasures(Matrix m) {
    size_t C = static_cast<size_t>(m.C);
    size_t R = static_cast<size_t>(m.R);
    std::vector<int> V(C);
    std::vector<bool> rows(R);
    std::vector<bool> cols(C);

    for (int c = 0; c < m.C; ++c)
    {
        rows.assign(R, false);
        cols.assign(C, false);
        Matrix::type *column = m[c];
        for (int i = 0; i < m.R; ++i)
        {
            if (column[i])
            {
                for (int j = 0; j < m.C; ++j)
                {
                    if (m[j][i])
                    {
                        cols[static_cast<size_t>(j)] = true;
                    }
                }
            }
        }
        for (int i = 0; i < m.R; ++i)
        {
            if (m[c][i])
            {
                for (int j = 0; j < m.C; ++j)
                {
                    if (cols[static_cast<size_t>(j)] && m[j][i])
                    {
                        rows[static_cast<size_t>(i)] = true;
                    }
                }
            }
        }

        V[static_cast<size_t>(c)] = 0;
        for (int i = 0; i < m.R; ++i)
        {
            if (!rows[static_cast<size_t>(i)])
            {
                continue;
            }
            for (int j = 0; j < m.C; ++j)
            {
                if (cols[static_cast<size_t>(j)] && !m[j][i])
                {
                    V[static_cast<size_t>(c)]++;
                }
            }
        }
    }
    return V;
}

void BabNode::calculate_void_measures() {
    std::vector<int> void_measures = voidMeasures(this->matrix);
    this->voids.clear();
    for (int i = 0; i < this->matrix.C; i++)
    {
        this->voids.push_back(std::make_pair(i, void_measures[static_cast<ulong>(i)]));
    }
}

bool BabNode::is_duplicatable(float z_up, std::set<std::set<int>>& branches, size_t id) {
    if (this->z_low  >= z_up)
    {
        return false;
    }
    int attribute = this->matrix.col_id[id];
    if (this->duplicated.find(attribute) != this->duplicated.end())
    {
        return false;
    }
    this->duplicated.insert(attribute);
    if (branches.find(this->duplicated) != branches.end())
    {
        this->duplicated.erase(attribute);
        return false;
    }
    else
    {
        int count = 0;
        for (int i = 0; i < this->matrix.R; i++)
        {
            if (this->matrix[static_cast<int>(id)][i] == 1)
            {
                count++;
            }
        }
        if (count < 2)
        {
            this->duplicated.erase(attribute);
            return false;
        }
        else
        {
            branches.insert(this->duplicated);
            this->duplicated.erase(attribute);
            return true;
        }
    }
}

Matrix select_minor(const Matrix& m, const std::vector<bool>& mr, const std::vector<bool>& mc) {
    int cCount = 0;
    int rCount = 0;

    if ((mr.size() == 0) && (mc.size() == 0))
    {
        std::cerr << "select_minor: empty minor\n";
        return Matrix(0,0);
    }
    for (auto i : mc)
    {
        if (i)
        {
            cCount++;
        }
    }
    for (auto i : mr)
    {
        if (i)
        {
            rCount++;
        }
    }
    Matrix minor = Matrix(rCount, cCount);
    int minor_i = 0;
    int minor_j = 0;
    for (size_t m_i = 0; static_cast<int>(m_i) < m.R; m_i++) {
        for (size_t m_j = 0; static_cast<int>(m_j) < m.C; m_j++) {
            if (mr[m_i] && mc[m_j]) {
                minor.row_id[minor_i] = m.row_id[m_i];
                minor.col_id[minor_j] = m.col_id[m_j];
                minor[minor_j][minor_i] = m[static_cast<int>(m_j)][m_i];
                minor_j++;
            }
        }
        if (minor_j != 0) {
            minor_i++;
            minor_j = 0;
        }
    }
    return minor;
}

Matrix select_adjunct_minor(const Matrix& m, const std::vector<bool>& mr, const std::vector<bool>& mc) {
    std::vector<bool> orthogonal_mr(0);
    std::vector<bool> orthogonal_mc(0);

    for (auto i : mr)
    {
        orthogonal_mr.push_back(!i);
    }
    for (auto i : mc)
    {
        orthogonal_mc.push_back(!i);
    }
    return select_minor(m, orthogonal_mr, orthogonal_mc);
}

std::pair<Matrix&&, Matrix&&> binary_split(const Matrix& m) {
    std::vector<bool> mr(static_cast<ulong>(m.R), false);
    std::vector<bool> mc(static_cast<ulong>(m.C), false);

    mr[0] = true;
    int rCount = 1;
    for (int i = 0; i < m.C; i++)
    {
        mc[static_cast<ulong>(i)] = m[i][0];
    }
    bool need_scan = true;
    while (need_scan)
    {
        need_scan = false;
        for (int r = 1; r < m.R; r++)
        {
            if (!mr[static_cast<ulong>(r)])
            {
                for (int j = 0; j < m.C; j++)
                {
                    if (mc[static_cast<ulong>(j)] && m[j][r])
                    {
                        for (int i = 0; i < m.C; i++)
                        {
                            mc[static_cast<ulong>(i)] = mc[static_cast<ulong>(i)] || m[i][r];
                        }
                        mr[static_cast<ulong>(r)] = true;
                        need_scan                 = true;
                        rCount++;
                        break;
                    }
                }
            }
        }
    }
    if (rCount == m.R)
    {
        std::cerr << "binat=ry_split: nothing to split\n";
        return std::pair<Matrix&&, Matrix&&> (Matrix(m), Matrix(0,0));
    }
    else
    {
        return std::pair<Matrix&&, Matrix&&> (select_minor(m,mr,mc), select_adjunct_minor(m,mr,mc));
    }
}
/*(
std::vector<Matrix>&& cluster_identification(Matrix& m) {
    Matrix mp                               = Matrix(m);
    std::vector<Matrix> list_of_submatrices = std::vector<Matrix>(0);

    while ((mp.R != 0) && (mp.C != 0))
    {
        std::pair<Matrix, Matrix> submatrices = binary_split(mp);
        if ((submatrices.second.R == 0) && (submatrices.second.C == 0) && (submatrices.first.cohesion() >= BabNode::threshold))
        {
            list_of_submatrices.push_back(submatrices.first);
            mp = submatrices->second;
        }
        else if (mp->cohesion() >= BabNode::threshold) {
            list_of_submatrices->push_back(mp);
            mp = nullptr;
        }
        else {
            list_of_submatrices->push_back(mp);
            return list_of_submatrices;
        }
    }
    return std::move(list_of_submatrices);
}
*/
//From Fig. 12. Algorithm for merging submatrices, line 6
Matrix&& merge_clusters(const Matrix& m1, const Matrix& m2) {
    std::set<int> rows;
    std::set<int> columns;

    for (int i = 0; i < m1.C; i++) {
        columns.emplace(m1.col_id[i]);
    }
    for (int i = 0; i < m2.C; i++) {
        columns.emplace(m2.col_id[i]);
    }
    for (int i = 0; i < m1.R; i++) {
        rows.emplace(m1.row_id[i]);
    }
    for (int i = 0; i < m2.R; i++) {
        rows.emplace(m2.row_id[i]);
    }

    Matrix output_matrix = Matrix(static_cast<int>(rows.size()), static_cast<int>(columns.size()));
    int row_it            = 0;
    int column_it         = 0;
    for (auto element : rows) {
        output_matrix.row_id[row_it] = element;
        row_it++;
    }
    for (auto element : columns) {
        output_matrix.col_id[column_it] = element;
        column_it++;
    }

    for (int i = 0; i < output_matrix.R; i++) {
        for (int j = 0; j < output_matrix.C; j++) {
            output_matrix[j][i] = 0;
            for (int m1_i = 0; m1_i < m1.R; m1_i++) {
                for (int m1_j = 0; m1_j < m1.C; m1_j++) {
                    if ((m1.row_id[m1_i] == output_matrix.row_id[i]) &&
                        (m1.col_id[m1_j] == output_matrix.col_id[j])) {
                        output_matrix[j][i] |= m1[m1_j][m1_i];
                    }
                }
            }

            for (int m2_i = 0; m2_i < m2.R; m2_i++) {
                for (int m2_j = 0; m2_j < m2.C; m2_j++) {
                    if ((m2.row_id[m2_i] == output_matrix.row_id[i]) &&
                        (m2.col_id[m2_j] == output_matrix.col_id[j])) {
                        output_matrix[j][i] |= m2[m2_j][m2_i];
                    }
                }
            }
        }
    }

    return std::move(output_matrix);
}

bool pair_is_empty(std::pair<int,int> p) {
    return (p.first == -1) && (p.second == -1);
}

Solution merge(std::vector<BabNode>& input_clusters) {
    std::vector<Matrix> output_clusters;
    //make list of matrices from list of BabNode pointers
    for (size_t i = 0; i < input_clusters.size(); i++)
    {
        output_clusters.push_back(input_clusters[i].matrix);
    }

    std::pair<int,int> merge_pair = std::make_pair(0,0);
    float cohesion_max  = 0.0;
    float cohesion_curr = 0.0;

    while (!pair_is_empty(merge_pair))
    {
        Matrix m_max(0,0);
        merge_pair    = std::make_pair(-1,-1);
        cohesion_curr = 0.0;
        for (size_t i = 0; i < output_clusters.size(); i++)
        {
            for (size_t j = i + 1; j < output_clusters.size(); j++)
            {
                Matrix m_current = merge_clusters(output_clusters[i], output_clusters[j]);
                cohesion_curr = m_current.cohesion();
                if ((cohesion_curr >= cohesion_max) && (cohesion_curr >= BabNode::threshold))
                {
                    cohesion_max = cohesion_curr;
                    m_max        = m_current;
                    merge_pair   = std::make_pair(i,j);
                }
            }
        }

        if (pair_is_empty(merge_pair))
        {
            break;
        }

        output_clusters.erase(output_clusters.begin() + std::max(merge_pair.first, merge_pair.second));
        output_clusters.erase(output_clusters.begin() + std::min(merge_pair.first, merge_pair.second));
        output_clusters.push_back(m_max);
    }

    return Solution(output_clusters);
}

Solution merge(std::vector<Matrix>& input_clusters) {
    std::pair<int,int> merge_pair = std::make_pair(0,0);
    float cohesion_max  = 0.0;
    float cohesion_curr = 0.0;
    std::vector<Matrix> clusters;
    for (auto m : input_clusters) {
        clusters.push_back(Matrix(m));
    }

    while (!pair_is_empty(merge_pair))
    {
        Matrix m_max(0,0);
        merge_pair    = std::make_pair(-1,-1);
        cohesion_curr = 0.0;
        for (size_t i = 0; i < clusters.size(); i++)
        {
            for (size_t j = i + 1; j < clusters.size(); j++)
            {
                Matrix m_current = merge_clusters(clusters[i], clusters[j]);
                cohesion_curr = m_current.cohesion();
                if ((cohesion_curr >= cohesion_max) && (cohesion_curr >= BabNode::threshold))
                {
                    cohesion_max = cohesion_curr;
                    m_max        = m_current;
                    merge_pair   = std::make_pair(i,j);
                }
            }
        }

        if (pair_is_empty(merge_pair))
        {
            break;
        }
        clusters.erase(clusters.begin() + std::max(merge_pair.first, merge_pair.second));
        clusters.erase(clusters.begin() + std::min(merge_pair.first, merge_pair.second));
        clusters.push_back(m_max);
    }
    return Solution(clusters);
}

bool is_feasible(const std::vector<Matrix>& clusters) {
    for (auto m : clusters)
    {
        if (m.cohesion() < BabNode::threshold || m.R == 0 || m.C == 0)
        {
            return false;
        }
        for (int i = 0; i < m.R; i++) {
            if (m.zeroRow(i))
            {
                return false;
            }
        }
        for (int i = 0; i < m.C; ++i)
        {
            if (m.zeroColumn(i))
            {
                return true;
            }
        }
    }
    return true;
}

Matrix&& duplicate(Matrix& m, int attribute) {
    if ((attribute < 0) || (m.C <= attribute))
    {
        std::cout << "duplicate: attribute_id not found\n";
        return std::move(Matrix(m));
    }
    //count number of "1" in attribute column
    int count = 0;
    for (int i = 0; i < m.R; i++)
    {
        if (m[attribute][i] != 0)
        {
            count++;
        }
    }

    if (count < 2) {
        return std::move(Matrix(m));
    }
    //number of new columns
    count--;
    //filling in not duplicated parts and assigning rows/cols id
    Matrix duplicate_m = Matrix(m.R, m.C + count);
    for (int i = 0; i < m.R; i++)
    {
        for (int j = 0; j < attribute; j++)
        {
            duplicate_m[j][i]     = m[j][i];
            duplicate_m.col_id[j] = m.col_id[j];
        }
        for (int j = attribute; j < attribute + count + 1; j++)
        {
            duplicate_m[j][i]     = 0;
            duplicate_m.col_id[j] = m.col_id[attribute];
        }
        for (int j = attribute + count + 1; j < m.C + count; j++)
        {
            duplicate_m[j][i]     = m[j - count][i];
            duplicate_m.col_id[j] = m.col_id[j - count];
        }
        duplicate_m.row_id[i] = m.row_id[i];
    }
    //filling in duplicated parts
    for (int i = 0, unit_i = 0; unit_i < count + 1; i++)
    {
        if (m[attribute][i] == 1)
        {
            duplicate_m[attribute + unit_i][i] = 1;
            unit_i++;
        }
    }
    return std::move(duplicate_m);
}

bool vois_measures_cmp(const std::pair<int,int> pair_1, const std::pair<int,int> pair_2) {
    return pair_1.second > pair_2.second;
}

Solution Bab11(Matrix m) {
    if (m.cohesion() >= BabNode::threshold) {
       return Solution(std::vector<Matrix>(1, m));
    }
    std::ofstream out("out");

    std::set<std::set<int>> branches;
    float z_up = std::numeric_limits<float>::max();
    std::stack<BabNode> stack_of_node;
    stack_of_node.push(BabNode(m, 0.0));
    std::vector<Matrix> incumbent_sol;
    std::vector<BabNode> list_of_curr_nodes(0);
    while(!stack_of_node.empty()) {
        BabNode current_node = std::move(stack_of_node.top());
        stack_of_node.pop();
        #ifdef DEBUG
        out
        << "current_node:\n" << current_node.matrix
        << "\nz_up: " << z_up << " z_low " << current_node.z_low <<  "\nCI:\n";
        #endif
        std::vector<Matrix> node_clusters = current_node.matrix.clusters();
        #ifdef DEBUG
        for (auto c : node_clusters)
        {
            out << c << "\n";
        }
        out << "END CI\n";
        out << "is_feasible: " << is_feasible(node_clusters) << "\n";
        #endif
        if ((is_feasible(node_clusters)) && (current_node.z_low < z_up))
        {
            #ifdef DEBUG
            out << "found\n";
            #endif
            incumbent_sol = std::move(node_clusters);
            z_up          = current_node.z_low;

            std::cout << "z_up: " << z_up << "incumbent:\n";
            for (auto mm : merge(incumbent_sol).clusters)
            {
                std::cout << mm << "\n";
            }
            out << "z_up: " << z_up << "\n";
        }
        else
        {
            if (current_node.z_low >= z_up) {
                #ifdef DEBUG
                out << "delete node\n";
                #endif
                continue;
            }
            list_of_curr_nodes.clear();
            current_node.calculate_void_measures();
            std::sort(current_node.voids.begin(), current_node.voids.end(), vois_measures_cmp);
            for (size_t j = 0; static_cast<int>(j) < current_node.matrix.C; j++)
            {
                size_t id = current_node.voids[j].first;
                #ifdef DEBUG
                out << "z_low: " << current_node.z_low << " j: " << j << "/" << current_node.matrix.C << " id: " << id << "\n";
                #endif
                if (current_node.is_duplicatable(z_up, branches, id))
                {
                    #ifdef DEBUG
                    out << "duplicating:\n\n";
                    #endif
                    Matrix duplicated = duplicate(current_node.matrix, static_cast<int>(id));
                    BabNode new_node(duplicated, current_node.z_low + 1);
                    new_node.duplicated = current_node.duplicated;
                    new_node.duplicated.insert(current_node.matrix.col_id[id]);
                    #ifdef DEBUG
                    out << new_node.matrix << "\n";
                    out << "new_node duplicated: ";
                    for (auto p : new_node.duplicated)
                    {
                        out << p << " ";
                    }
                    out << "\n";
                    #endif
                    list_of_curr_nodes.push_back(std::move(new_node));
                }
            }
            for (int i = 0; static_cast<ulong>(i) < list_of_curr_nodes.size(); i++) {
                stack_of_node.push(std::move(list_of_curr_nodes[static_cast<ulong>(i)]));
            }
        }
    }
    #ifdef DEBUG
    out << "sol:\n";
    #endif
    if (incumbent_sol.size() == 0)
    {
        std::cout << "no clusters\n";
        return Solution();
    }
    #ifdef DEBUG
    for (auto sol : merge(incumbent_sol).clusters)
    {
        out << sol << "\n";
    }
    #endif
    return merge(incumbent_sol);
}
