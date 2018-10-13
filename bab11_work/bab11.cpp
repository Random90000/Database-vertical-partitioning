
#include "bab11.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <unordered_map>

#define DEBUG

float BabNode::threshold = 0.7;

BabNode::BabNode()
    :matrix(Matrix(0,0)), level(0){}

BabNode::BabNode(const Matrix m, float z_low, int level)
    :matrix(m), z_low(z_low), voids(std::vector<std::pair<size_t,int>>(0)), level(level){}

BabNode::BabNode(BabNode&& other)
    :matrix(std::move(other.matrix)), z_low(std::move(other.z_low)),
     voids(std::move(other.voids)), duplicated(std::move(other.duplicated)), level(other.level){}

BabNode::BabNode(const BabNode& other)
    :matrix(other.matrix), z_low(other.z_low), voids(other.voids), duplicated(other.duplicated), level(other.level){}

BabNode::~BabNode(){}

BabNode& BabNode::operator=(BabNode&& other){
    this->matrix     = std::move(other.matrix);
    this->z_low      = std::move(other.z_low);
    this->voids      = std::move(other.voids);
    this->duplicated = std::move(other.duplicated);
    this->level = other.level;
    return *this;
}

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
    Matrix selecting_minor(rCount, cCount);
    int minor_i = 0;
    int minor_j = 0;
    for (size_t m_i = 0; static_cast<int>(m_i) < m.R; m_i++) {
        for (size_t m_j = 0; static_cast<int>(m_j) < m.C; m_j++) {
            if (mr[m_i] && mc[m_j]) {
                selecting_minor.row_id[minor_i] = m.row_id[m_i];
                selecting_minor.col_id[minor_j] = m.col_id[m_j];
                selecting_minor[minor_j][minor_i] = m[static_cast<int>(m_j)][m_i];
                minor_j++;
            }
        }
        if (minor_j != 0) {
            minor_i++;
            minor_j = 0;
        }
    }
    return selecting_minor;
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

std::pair<Matrix, Matrix> binary_split(const Matrix& m) {
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
        std::cerr << "binary_split: nothing to split\n";
        return std::pair<Matrix, Matrix> (Matrix(m), Matrix(0,0));
    }
    else
    {
        return std::pair<Matrix, Matrix> (select_minor(m,mr,mc), select_adjunct_minor(m,mr,mc));
    }
}

std::ostream& operator<<(std::ostream& out, std::map<int,std::pair<std::vector<int>, std::vector<int>>> m){
    for (auto i : m)
    {
        out << i.first << " [";
        for (auto j : i.second.first)
        {
            out << j << " ";
        }
        out << "] [";
        for (auto j : i.second.second)
        {
            out << j << " ";
        }
        out << "]\n";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, std::map<int,std::pair<int,int>> m)
{
    for (auto i : m)
    {
        out << " " << i.first << " <" << i.second.first << " " << i.second.second << ">";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, std::vector<Matrix> v)
{
    std::cout << "clusters:\n";
    for (auto t : v)
    {
        std::cout << t << "\n";
    }
    return out;
}

//From Fig. 12. Algorithm for merging submatrices, line 6
Matrix merge_clusters(const Matrix& m1, const Matrix& m2) {
    std::map<int,std::pair<std::vector<int>, std::vector<int>>> columns;
    std::map<int,std::pair<int,int>> rows;
    for (int i = 0; i < m1.C; i++)
    {
        columns[m1.col_id[i]].first.push_back(i);
    }
    for (int i = 0; i < m2.C; i++)
    {
        columns[m2.col_id[i]].second.push_back(i);
    }
    //std::cout << "columns:\n" << columns << "\n";
    for (int i = 0; i < m1.R; i++)
    {
        if (rows.find(m1.row_id[i]) == rows.end())
        {
            rows[m1.row_id[i]] = std::make_pair(-1,-1);
        }
        rows[m1.row_id[i]].first = i;
    }
    for (int i = 0; i < m2.R; i++)
    {
        if (rows.find(m2.row_id[i]) == rows.end())
        {
            rows[m2.row_id[i]] = std::make_pair(-1,-1);
        }
        rows[m2.row_id[i]].second = i;
    }
    //std::cout << "rows:\n" << rows << "\n";
    Matrix output_matrix(static_cast<int>(rows.size()), static_cast<int>(columns.size()));
    int i = 0;
    for (auto row_it : rows)
    {
        output_matrix.row_id[i] = row_it.first;
        i++;
    }
    i = 0;
    for (auto col_it : columns)
    {
        output_matrix.col_id[i] = col_it.first;
        i++;
    }

    auto row_it = rows.begin();
    for (int r = 0; r < output_matrix.R; r++)
    {
        auto col_it = columns.begin();
        for (int c = 0; c < output_matrix.C; c++)
        {
            output_matrix[c][r] = 0;
            if (row_it->second.first != -1)
            {
                for (auto col : col_it->second.first)
                {
                    output_matrix[c][r] |= m1[col][row_it->second.first];
                }
            }
            if (output_matrix[c][r] == 1)
            {
                col_it++;
                continue;
            }
            if (row_it->second.second != -1)
            {
                for (auto col : col_it->second.second)
                {
                    output_matrix[c][r] |= m2[col][row_it->second.second];
                }
            }
            col_it++;
        }
        row_it++;
    }
    return output_matrix;
}

bool pair_is_empty(std::pair<int,int> p) {
    return (p.first == -1) && (p.second == -1);
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

Matrix duplicate(Matrix& m, int attribute) {
    if ((attribute < 0) || (m.C <= attribute))
    {
        std::cout << "duplicate: attribute_id not found\n";
        return m;
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
        return m;
    }
    //number of new columns
    count--;
    //filling in not duplicated parts and assigning rows/cols id
    Matrix duplicate_m(m.R, m.C + count);
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
    return duplicate_m;
}

int void_measures_sum(const BabNode node){
    int sum = 0;
    for (auto i : node.voids)
    {
        sum += i.second;
    }
    return sum;
}

bool void_measures_cmp(const BabNode node_1, const BabNode node_2) {
    return void_measures_sum(node_1) > void_measures_sum(node_2);
}

Solution Bab11(const Matrix& m) {
    if (m.cohesion() >= BabNode::threshold) {
       return Solution(std::vector<Matrix>(1, m));
    }
    std::ofstream out("out");

    std::set<std::set<int>> branches;
    float z_up = std::numeric_limits<float>::max();
    std::stack<BabNode> stack_of_node;
    stack_of_node.push(BabNode(m, 0.0));
    std::vector<Matrix> incumbent_sol;
    std::vector<BabNode> list_of_curr_nodes;
    while(!stack_of_node.empty()) {
        BabNode current_node = std::move(stack_of_node.top());

        out << "level: " << current_node.level << " stack: " << stack_of_node.size() << " z_low: " << current_node.z_low << " z_up: " << z_up << "\n"
            << current_node.matrix << "\n" ;

        stack_of_node.pop();
        std::vector<Matrix> node_clusters = current_node.matrix.clusters();

        out << "clusters: " << node_clusters.size() << "\n";
        if (node_clusters.size() > 4)
        {
            std::cout << "clusters:\n";
            for (auto l : node_clusters)
            {
                std::cout << l << "\n";
            }
            std::cout << "\n";
        }

        list_of_curr_nodes.clear();
        if ((is_feasible(node_clusters)) && (current_node.z_low < z_up))
        {
            incumbent_sol = std::move(node_clusters);
            z_up          = current_node.z_low;
        }
        else
        {
            if (current_node.z_low >= z_up)
            {
                continue;
            }
            current_node.calculate_void_measures();
            for (size_t j = 0; static_cast<int>(j) < current_node.matrix.C; j++)
            {
                size_t id = current_node.voids[j].first;
                if (current_node.is_duplicatable(z_up, branches, id))
                {
                    Matrix duplicated = duplicate(current_node.matrix, static_cast<int>(id));
                    BabNode new_node(duplicated, current_node.z_low + 1, current_node.level + 1);
                    //std::cout << "new node: " << new_node.level << "\n";
                    new_node.duplicated = current_node.duplicated;
                    new_node.duplicated.insert(current_node.matrix.col_id[id]);
                    new_node.calculate_void_measures();
                    list_of_curr_nodes.push_back(std::move(new_node));
                }
            }
            std::sort(list_of_curr_nodes.rbegin(), list_of_curr_nodes.rend(), void_measures_cmp);
            out  << "nodes count: " << list_of_curr_nodes.size() << " brances: " << branches.size() << "\n";
            for (int i = 0; static_cast<ulong>(i) < list_of_curr_nodes.size(); i++)
            {
                //std::cout << list_of_curr_nodes[static_cast<ulong>(i)].matrix << "\n";
                stack_of_node.push(std::move(list_of_curr_nodes[static_cast<ulong>(i)]));
            }
        }
    }
    if (incumbent_sol.size() == 0)
    {
        std::cout << "no clusters\n";
        return Solution();
    }
    return merge(incumbent_sol);
}
