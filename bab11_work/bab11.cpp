
#include "bab11.hpp"
#include <iostream>

float BabNode::threshold = 0.0;

BabNode::BabNode(Matrix* m, float z_low = 0.0, std::set<int>* duplicated = nullptr)
    :matrix(new Matrix(*m)), z_low(z_low), duplicated(new std::set<int>(*duplicated)) {}

BabNode::~BabNode() {
    delete this->matrix;
    delete this->duplicated;
}

void BabNode::calculate_void_measures() {
    std::vector<int> void_measures = voidMeasure(*this->matrix);

    if (this->voids != nullptr) {
        delete [] this->voids;
    }
    this->voids = new std::vector<std::pair<int,int>>(static_cast<ulong>(this->matrix->C));

    for (int i = 0; i < this->matrix->C; i++) {
        this->voids->push_back(std::make_pair(i, void_measures[static_cast<ulong>(i)]));
    }
}

bool BabNode::isBranching() {
    return branchingId != -1;
}

Matrix* select_minor(const Matrix* m, const std::vector<bool>* mr, const std::vector<bool>* mc) {
    int cCount = 0;
    int rCount = 0;

    if ((mr->size() == 0) && (mc->size() == 0)) {
        return nullptr;
    }
    for (auto i : *mc) {
        if (i) {
            cCount++;
        }
    }
    for (auto i : *mr) {
        if (i) {
            rCount++;
        }
    }
    Matrix* minor = new Matrix(rCount, cCount);
    int minor_i = 0;
    int minor_j = 0;
    for (size_t m_i = 0; static_cast<int>(m_i) < m->R; m_i++) {
        for (size_t m_j = 0; static_cast<int>(m_j) < m->C; m_j++) {
            if ((*mr)[m_i] && (*mc)[m_j]) {
                minor->row_id[minor_i] = m->row_id[m_i];
                minor->col_id[minor_j] = m->col_id[m_j];
                (*minor)[minor_j][minor_i] = (*m)[static_cast<int>(m_j)][m_i];
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

Matrix* select_adjunct_minor(const Matrix* m, const std::vector<bool>* mr, const std::vector<bool>* mc) {
    std::vector<bool> orthogonal_mr(0);
    std::vector<bool> orthogonal_mc(0);

    for (auto i : *mr) {
        orthogonal_mr.push_back(!i);
    }
    for (auto i : *mc) {
        orthogonal_mc.push_back(!i);
    }
    return select_minor(m, &orthogonal_mr, &orthogonal_mc);
}

std::pair<Matrix*, Matrix*>* binary_split(const Matrix* m) {
    std::vector<bool> mr(static_cast<ulong>(m->R), false);
    std::vector<bool> mc(static_cast<ulong>(m->C), false);

    mr[0] = true;
    int rCount = 1;
    for (int i = 0; i < m->C; i++) {
        mc[static_cast<ulong>(i)] = (*m)[i][0];
    }
    bool need_scan = true;
    while (need_scan) {
        need_scan = false;

        for (int r = 1; r < m->R; r++) {
            if (!mr[static_cast<ulong>(r)]) {
                for (int j = 0; j < m->C; j++) {
                    if (mc[static_cast<ulong>(j)] && (*m)[j][r]) {
                        for (int i = 0; i < m->C; i++) {
                            mc[static_cast<ulong>(i)] = mc[static_cast<ulong>(i)] || (*m)[i][r];
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
    if (rCount == m->R) {
        return nullptr;
    }
    else {
        return new std::pair<Matrix*, Matrix*> (select_minor(m,&mr,&mc), select_adjunct_minor(m,&mr,&mc));
    }
}

std::vector<Matrix*>* cluster_identification(const Matrix* m) {
    Matrix* mp                                = new Matrix(*m);
    std::vector<Matrix*>* list_of_submatrices = new std::vector<Matrix*>(0);

    while (mp != nullptr) {
        std::pair<Matrix*, Matrix*>* submatrices = binary_split(mp);
        if ((submatrices != nullptr) && (submatrices->first->cohesion() >= BabNode::threshold)) {
            list_of_submatrices->push_back(submatrices->first);
            delete mp;
            mp = submatrices->second;
        }
        else if (mp->cohesion() >= BabNode::threshold) {
            list_of_submatrices->push_back(mp);
            mp = nullptr;
        }
        else {
            delete submatrices;
            list_of_submatrices->push_back(mp);
            return list_of_submatrices;
        }
        delete submatrices;
    }
    delete mp;
    return list_of_submatrices;
}

//From Fig. 12. Algorithm for merging submatrices, line 6
Matrix* merge_clusters(const Matrix* m1, const Matrix* m2) {
    std::set<int> rows;
    std::set<int> columns;

    int tt = 0;

    std::cout << "clusters " << tt << "\n" << std::flush;

    for (int i = 0; i < m1->C; i++) {
        columns.emplace(m1->col_id[i]);
    }
    for (int i = 0; i < m2->C; i++) {
        columns.emplace(m2->col_id[i]);
    }
    for (int i = 0; i < m1->R; i++) {
        rows.emplace(m1->row_id[i]);
    }
    for (int i = 0; i < m2->R; i++) {
        rows.emplace(m2->row_id[i]);
    }

    std::cout << "clusters " << tt << "\n" << std::flush;

    Matrix* output_matrix = new Matrix(static_cast<int>(rows.size()), static_cast<int>(columns.size()));
    int row_it            = 0;
    int column_it         = 0;
    for (auto element : rows) {
        output_matrix->row_id[row_it] = element;
        row_it++;
    }
    for (auto element : columns) {
        output_matrix->col_id[column_it] = element;
        column_it++;
    }

    std::cout << "clusters " << tt << "\n" << std::flush;

    for (int i = 0; i < output_matrix->R; i++) {
        for (int j = 0; j < output_matrix->C; j++) {
            (*output_matrix)[j][i] = 0;
              for (int m1_i = 0; m1_i < m1->R; m1_i++) {
              /*  for (int m1_j = 0; m1_j < m1->C; m1_j++) {
                    if ((m1->row_id[m1_i] == output_matrix->row_id[i]) &&
                        (m1->col_id[m1_j] == output_matrix->col_id[j])) {
                        (*output_matrix)[j][i] |= (*m1)[m1_j][m1_i];
                    }
                }*/
            }

            for (int m2_i = 0; m2_i < m2->R; m2_i++) {
               /* for (int m2_j = 0; m2_j < m2->C; m2_j++) {
                    if ((m2->row_id[m2_i] == output_matrix->row_id[i]) &&
                        (m2->col_id[m2_j] == output_matrix->col_id[j])) {
                        (*output_matrix)[j][i] |= (*m2)[m2_j][m2_i];
                    }
                }*/
            }
        }
    }

    std::cout << "clusters " << tt << "\n" << std::flush;
    tt++;
    return output_matrix;
}

bool pair_is_empty(std::pair<int,int> p) {
    return (p.first == -1) && (p.second == -1);
}

Solution merge(std::vector<BabNode*>* input_clusters) {
    std::vector<Matrix> output_clusters;
    //make list of matrices from list of BabNode pointers
    std::cout << "merge 1\n" << std::flush;
    for (size_t i = 0; i < input_clusters->size(); i++) {
        if ((*input_clusters)[i] != nullptr) {
            output_clusters.push_back((*(*input_clusters)[i]->matrix));
        }
    }
    /*std::cout << "\nclusters\n";
    for (auto t : output_clusters){
        std::cout << t << "\n";
    }*/
    std::pair<int,int> merge_pair = std::make_pair(0,0);
    float cohesion_max  = 0.0;
    float cohesion_curr = 0.0;

    int mm = 0;
    std::cout << "merge 2\n" << std::flush;
    while (!pair_is_empty(merge_pair)) {
        Matrix* m_current = nullptr;
        Matrix m_max(0,0);

        std::cout << "merge " << mm << "\n" << std::flush;

        merge_pair    = std::make_pair(-1,-1);
        cohesion_curr = 0.0;
        for (size_t i = 0; i < output_clusters.size(); i++) {
            for (size_t j = i + 1; j < output_clusters.size(); j++) {
                m_current = merge_clusters(&output_clusters[i], &output_clusters[j]);
                cohesion_curr = m_current->cohesion();
                if ((cohesion_curr >= cohesion_max) && (cohesion_curr >= BabNode::threshold)) {
                    cohesion_max = cohesion_curr;
                    m_max        = *m_current;
                    merge_pair = std::make_pair(i,j);
                }

                std::cout << "c " << i << " " << j << "\n" << std::flush;

            }
        }

        std::cout << "merge " << mm << "\n" << std::flush;

        mm++;
        if (pair_is_empty(merge_pair)) {
            break;
        }
        /*std::cout << merge_pair.first << " " << merge_pair.second << "\nm_max:\n" << m_max << "\n";
        std::cout << "one\n";
        for (int i = 0; i < output_clusters.size(); i++) {
            std::cout << "no: " << i << "\n" << output_clusters[i] << "\n";
        }
        std::cout << "two\n";
        */output_clusters.erase(output_clusters.begin() + std::max(merge_pair.first, merge_pair.second));
        /*for (int i = 0; i < output_clusters.size(); i++) {
            std::cout << "no: " << i << "\n" << output_clusters[i] << "\n";
        }*/
        output_clusters.erase(output_clusters.begin() + std::min(merge_pair.first, merge_pair.second));
        output_clusters.push_back(m_max);
        /*for (int i = 0; i < output_clusters.size(); i++) {
            std::cout << "no: " << i << "\n" << output_clusters[i] << "\n";
        }*/

        std::cout << "merge " << mm << "\n" << std::flush;

        delete m_current;
    }
    return Solution(output_clusters);
}

bool is_feasible(const std::vector<Matrix*>* clusters) {
    for (auto m : *clusters) {
        if (m->cohesion() < BabNode::threshold || m->R == 0 || m->C == 0) {
            return false;
        }
        for (int i = 0; i < m->R; i++) {
            if (m->zeroRow(i)) {
                return false;
            }
        }
        for (int i = 0; i < m->C; ++i) {
            if (m->zeroColumn(i)) {
                return true;
            }
        }
    }
    return true;
}

Matrix* duplicate(Matrix* m, int attribute) {
    //find index of attribute in col_id
    int attribute_id = -1;
    for (int i = 0; i < m->C; i++) {
        if (m->col_id[i] == attribute) {
            attribute_id = i;
        }
    }
    if (attribute_id == -1) {
        return nullptr;
    }
    //count number of "1" in attribute column
    int count = 0;
    for (int i = 0; i < m->R; i++) {
        if ((*m)[attribute_id][i] != 0) {
            count++;
        }
    }
    if (count < 2) {
        return nullptr;
    }
    //number of new columns
    count--;
    //filling in not duplicated parts and assigning rows/cols id
    Matrix* duplicate_m = new Matrix(m->R, m->C + count);
    for (int i = 0; i < m->R; i++) {
        for (int j = 0; j < attribute_id; j++) {
            (*duplicate_m)[j][i]   = (*m)[j][i];
            duplicate_m->col_id[j] = m->col_id[j];
        }
        for (int j = attribute_id; j < attribute_id + count + 1; j++) {
            (*duplicate_m)[j][i]   = 0;
            duplicate_m->col_id[j] = attribute;
        }
        for (int j = attribute_id + count + 1; j < m->C + count; j++) {
            (*duplicate_m)[j][i]   = (*m)[j - count][i];
            duplicate_m->col_id[j] = m->col_id[j - count];
        }
        duplicate_m->row_id[i] = m->row_id[i];
    }
    //filling in duplicated parts
    for (int i = 0, unit_i = 0; unit_i < count + 1; i++) {
        if ((*m)[attribute_id][i] == 1) {
            (*duplicate_m)[attribute_id + unit_i][i] = 1;
            unit_i++;
        }
    }
    return duplicate_m;
}

bool vois_measures_cmp(const std::pair<int,int> pair_1, const std::pair<int,int> pair_2) {
    return pair_1.second > pair_2.second;
}
/*
Solution Bab11(const Matrix* m) {
    if (m->cohesion() >= BabNode::threshold) {
       return Solution(std::vector<Matrix>(1, *m)); 
    }

    float z_up = std::numeric_limits<float>::max();
    std::stack<BabNode*>* stack_of_node = new std::stack<BabNode*>();
    stack_of_node->push(new BabNode(m));
    BabNode* current_node               = nullptr;
    std::vector<Matrix*>* node_clusters = nullptr;
    std::vector<Matrix*>* incumbent_sol = nullptr;
    std::vector<BabNode*>* list_of_curr_nodes = new std::vector<BabNode*>(0); 
    
    while(!stack_of_node->empty()) {
        current_node  = stack_of_node->top();
        std::vector<bool> duplicated_attributes(false, current_node->matrix->C);        
        stack_of_node->pop();
        node_clusters = cluster_identification(current_node->matrix);
        if ((is_feasible(node_clusters)) && (current_node->z_low < z_up)) {
            incumbent_sol = node_clusters;
            z_up          = current_node->z_low;
        }
        else {
            if (current_node->z_low >= z_up) {
                delete current_node;
                continue;
            }
            list_of_curr_nodes->clear();
            current_node->calculate_void_measures();
            std::sort(current_node->voids->begin(), current_node->voids->end());
            for (int j = 0; j < current_node->matrix->C; j++) {
                int column = (*current_node->voids)[static_cast<ulong>(j)].first;
                if (!current_node->matrix->zeroColumn(column)) {
                    BabNode* new_node = new BabNode(duplicate(current_node->matrix, column),
                                                    current_node->z_low + 1,
                                                    current_node->duplicated);
                    new_node->duplicated->insert(column);
                    list_of_curr_nodes->push_back(new_node);
                }
            }
            for (int i = 0; static_cast<ulong>(i) < list_of_curr_nodes->size(); i++) {
                stack_of_node->push((*list_of_curr_nodes)[static_cast<ulong>(i)]);
            }
        }
    }
}
*/
