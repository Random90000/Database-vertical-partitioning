#include "bab11.hpp"
#include "measures.hpp"

#include <fstream>

float BabNode::threshold = 0.6;

BabNode::BabNode()
    :matrix(Matrix(0,0)){}

BabNode::BabNode(const Matrix m, float z_low, size_t max_duplicated_id)
    :matrix(m), max_duplicated_id(max_duplicated_id), z_low(z_low), voids(std::vector<std::pair<size_t,int>>(0)){}

BabNode::BabNode(BabNode&& other)
    :matrix(std::move(other.matrix)), max_duplicated_id(other.max_duplicated_id), z_low(other.z_low), voids(std::move(other.voids)){}

BabNode::BabNode(const BabNode& other)
    :matrix(other.matrix), max_duplicated_id(other.max_duplicated_id), z_low(other.z_low), voids(other.voids){}

BabNode::~BabNode(){}

BabNode& BabNode::operator=(BabNode&& other){
    this->matrix            = std::move(other.matrix);
    this->z_low             = other.z_low;
    this->voids             = std::move(other.voids);
    this->max_duplicated_id = other.max_duplicated_id;
    return *this;
}

std::vector<int> voidMeasures(Matrix m){
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

void BabNode::calculate_void_measures(){
    std::vector<int> void_measures = voidMeasures(this->matrix);
    this->voids.clear();
    std::for_each(void_measures.begin(), void_measures.end(), [&,i{0}](auto vm) mutable {voids.push_back({i,vm}); i++;});
}

void BabNode::sort_void_measures(){
    std::sort(this->voids.begin(),this->voids.end(),[&](const auto p1, const auto p2){return p1.second < p2.second;});
}

//branches - sets of already duplicated attributes in matrix from Bab11 function; id - verifiable attribute from matrix

bool BabNode::is_duplicatable(float z_up, size_t id) {

    //std::cout << "try: " << this->z_low << " " << id << "\n" << this->matrix << "\n";

    if ((this->z_low  >= z_up) || (id < this->max_duplicated_id))
    {
        return false;
    }
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
        return false;
    }
    //std::cout << "ok\n";
    return true;
}

Matrix select_minor(const Matrix& m, const std::vector<bool>& mr, const std::vector<bool>& mc) {
    int cCount = 0;
    int rCount = 0;

    if ((mr.size() == 0) && (mc.size() == 0))
    {
        return Matrix(0,0);
    }
    std::for_each(mc.begin(), mc.end(), [&](auto i){if (i) cCount++;});
    std::for_each(mr.begin(), mr.end(), [&](auto i){if (i) rCount++;});

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

    std::for_each(mc.begin(), mc.end(), [&](auto i){orthogonal_mc.push_back(!i);});
    std::for_each(mr.begin(), mr.end(), [&](auto i){orthogonal_mr.push_back(!i);});

    return select_minor(m, orthogonal_mr, orthogonal_mc);
}

Matrix compression(const Matrix& m){
    std::map<int,std::set<int>> attributes;//<col_id,indexes of duplicated attributes in m>
    for (int i = 0; i < m.C; i++)
    {
        attributes[m.col_id[i]].insert(i);
    }
    Matrix compressed(m.R, static_cast<int>(attributes.size()));
    for (int i = 0; i < m.R; i++)
    {
        compressed.row_id[i] = m.row_id[i];
    }
    std::for_each(attributes.begin(), attributes.end(), [&,i{0}](auto it) mutable {compressed.col_id[i] = it.first; i++;});
    for (int i = 0; i < m.R; i++)
    {
        for (int j = 0; j < static_cast<int>(attributes.size()); j++)
        {
            compressed[j][i] = 0;
            auto set_it = attributes[compressed.col_id[j]];
            std::for_each(set_it.begin(), set_it.end(), [&](auto it){compressed[j][i] |= m[it][i];});
        }
    }
    return compressed;
}

std::pair<Matrix, Matrix> binary_split(const Matrix& m) {
    if ((m.C == 0) || (m.R == 0))
    {
        std::cerr << "\nbinary_split: empty matrix\n";
    }
    std::vector<bool> mr(static_cast<ulong>(m.R), false);
    std::vector<bool> mc(static_cast<ulong>(m.C), false);

    mr[0] = true;
    int rCount = 1;
    std::for_each(mc.begin(), mc.end(), [&,i{0}](auto val) mutable {val = m[i][0]; i++;});
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
                        std::for_each(mc.begin(),mc.end(),[&,i{0}](auto val) mutable {val = val || m[i][r]; i++;});
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
        return std::pair<Matrix, Matrix> (compression(m), Matrix(0,0));
    }
    else
    {
        return std::pair<Matrix, Matrix> (compression(select_minor(m,mr,mc)), select_adjunct_minor(m,mr,mc));
    }
}

std::vector<Matrix> cluster_identification(const Matrix& m) {
    Matrix mp = Matrix(m);
    std::vector<Matrix> list_of_submatrices;

    while ((mp.R != 0) && (mp.C != 0))
    {
        std::pair<Matrix, Matrix> submatrices = binary_split(mp);
        if (submatrices.first.cohesion() >= BabNode::threshold)
        {
            list_of_submatrices.push_back(submatrices.first);
            mp = submatrices.second;
        }
        else if (mp.cohesion() >= BabNode::threshold)
        {
            list_of_submatrices.push_back(mp);
        }
        else
        {
            list_of_submatrices.push_back(mp);
            return list_of_submatrices;
        }
    }
    return list_of_submatrices;
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

Solution merge(std::vector<Matrix> input_clusters) {
    std::pair<int,int> merge_pair = std::make_pair(0,0);
    float cohesion_max  = BabNode::threshold;
    float cohesion_curr = 0.0;
    std::vector<Matrix> clusters(input_clusters);

    while (!pair_is_empty(merge_pair))
    {
        Matrix m_max(0,0);
        merge_pair   = std::make_pair(-1,-1);
        cohesion_max = BabNode::threshold;
        for (size_t i = 0; i < clusters.size(); i++)
        {
            for (size_t j = i + 1; j < clusters.size(); j++)
            {

                Matrix m_current = merge_clusters(clusters[i], clusters[j]);
                cohesion_curr    = m_current.cohesion();

                //std::cout << "try curr " << cohesion_curr << " max " << cohesion_max << "\n" << clusters[i] << "\n" << clusters[j] << "\n";

                if (cohesion_curr >= cohesion_max)
                {
                    cohesion_max = cohesion_curr;
                    m_max        = m_current;
                    merge_pair   = std::make_pair(i,j);

                    //std::cout << "curr max\n" << m_max << "\n";
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

        std::cout << "max\n" << m_max << "\n" << "merge_pair " << merge_pair.first << " " << merge_pair.second << " " << pair_is_empty(merge_pair) <<"\n";
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
        std::cerr << "duplicate: attribute_id not found\n";
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

Solution Bab11(const Matrix& m) {

    std::ofstream out("out");

    if (m.cohesion() >= BabNode::threshold)
    {
       return Solution(std::vector<Matrix>(1, m));
    }
    std::set<std::set<int>> branches;
    float z_up = std::numeric_limits<float>::max();
    std::stack<BabNode> stack_of_node;
    stack_of_node.push(BabNode(m, 0.0));
    std::vector<Matrix> incumbent_sol;
    std::vector<BabNode> list_of_curr_nodes;

    int max_z = 0;

    int nodes_count = 0;

    while(!stack_of_node.empty())
    {
        BabNode current_node = std::move(stack_of_node.top());
        stack_of_node.pop();
        std::vector<Matrix> node_clusters = cluster_identification(current_node.matrix);
        list_of_curr_nodes.clear();

        if (current_node.z_low > max_z)
        {
            std::cout << "max: " << current_node.z_low << "\n";
            max_z = current_node.z_low;
        }

        //out << "level: " <<  current_node.z_low << " duplicated: " << current_node.max_duplicated_id <<  " matrix:\n" << current_node.matrix << "\n";
        //for (auto k : node_clusters)
        //{
        //    out << k << "\n";
        //}

        if (is_feasible(node_clusters) && (current_node.z_low < z_up))
        {
            incumbent_sol = std::move(node_clusters);
            z_up          = current_node.z_low;
            //std::cout << "branches size: " << branches.size() << " nodes_count: " << nodes_count << "\n";
            std::cout << "solution:\n";
            for (auto i : incumbent_sol)
            {
                std::cout << "cohesion: " << i.cohesion() << " z_low " << current_node.z_low << "\n" << i << "\n";
            }
        }
        else
        {
            if (current_node.z_low >= z_up)
            {
                continue;
            }
            nodes_count++;
            current_node.calculate_void_measures();
            current_node.sort_void_measures();
            /*std::cout << "voids: ";
            for (auto l : current_node.voids)
            {
                std::cout << l.first << "," << l.second << " ";
            }
            std::cout << "\n";*/
            std::for_each(current_node.voids.begin(),current_node.voids.end(),
                          [&](auto p){
                              size_t id = p.first;
                              if (current_node.is_duplicatable(z_up, id))
                              {
                                  BabNode new_node(duplicate(current_node.matrix, static_cast<int>(id)),
                                                   current_node.z_low + 1,
                                                   id
                                                   //current_node.duplicated,
                                                   );
                                  //new_node.duplicated.insert(current_node.matrix.col_id[id]);
                                  list_of_curr_nodes.push_back(new_node);
                              }
                            }
                          );
            for (int i = 0; static_cast<ulong>(i) < list_of_curr_nodes.size(); i++)
            {
                stack_of_node.push(std::move(list_of_curr_nodes[static_cast<ulong>(i)]));
            }
        }
    }
    std::cout << "nodes_count " << nodes_count << "\n";
    //std::cout << "branches size: " << branches.size() << " nodes_count: " << nodes_count << "\n";
    if (incumbent_sol.size() == 0)
    {
        return Solution({m});
    }
    std::cout << "result solution:\n";
    for (auto i : incumbent_sol)
    {
        std::cout << i << "\n";
    }
    return merge(incumbent_sol);
}
