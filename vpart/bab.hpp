#pragma once

#include <algorithm>
#include <limits>
#include <set>
#include <stack>
#include <vector>

#include "matrix.hpp"
#include "measures.hpp"

extern int nodes;

template<typename Type, bool minimizing> struct Strategy {
    Strategy(){}
    Strategy(const Strategy& other){
        submatrices = other.submatrices;
        intercluster = other.intercluster;
        submatrix = other.submatrix;
    }
    Strategy(Strategy&& other){
        submatrices = std::move(other.submatrices);
        intercluster = std::move(other.intercluster);
        submatrix = other.submatrix;
    }
    std::vector<Matrix> submatrices;
    std::set<int> intercluster;
    int submatrix = -1;
    using bound_type = Type;
    static bound_type init_bound(){
        if(minimizing) return std::numeric_limits<Type>::max();
        return 0;
    }
    static bool is_better(bound_type current, bound_type best){
        if(minimizing) return current < best;
        else return current > best;
    }
    bool is_branching(){ return submatrix != -1; }
    void start_branching(int i){ submatrix = i; }
    std::vector<Matrix>& subs(){ return submatrices; }
    bool has_empty_rows_cols(){
        for(auto& m : submatrices){
            if(m.R == 0 || m.C == 0) return true;
            for(int i = 0; i < m.R; ++i)
                if(m.zeroRow(i)) return true;
            for(int i = 0; i < m.C; ++i)
                if(m.zeroColumn(i)) return true;
        }
        return false;
    }
};

struct Solution {
    std::vector<Matrix> clusters;
    std::set<int> intercluster;
    Solution(){}
    template<typename Type, bool minimizing> Solution(Strategy<Type, minimizing>&& strat){
        clusters = std::move(strat.submatrices);
        intercluster = std::move(strat.intercluster);
    }
    template<typename Type, bool minimizing> Solution& operator=(Strategy<Type, minimizing>&& strat){
        clusters = std::move(strat.submatrices);
        intercluster = std::move(strat.intercluster);
        return *this;
    }

    Solution(std::vector<Matrix> clusters) {
        this->clusters = std::move(clusters);
    }
};

// having thresholds as static members is good performance- and memory-wise
// but bad everything-else-wise

struct MaxRowsBased {
    static int threshold;
    static bool valid_submatrix(const Matrix& m){
        return m.R <= threshold;
    }
};

struct CohesionBased {
    static float threshold;
    static bool valid_submatrix(const Matrix& m){
        #ifdef DEBUG
        std::cerr << "cohesion: " << m.cohesion() << std::endl;
        #endif
        return m.cohesion() >= threshold;
    }
};

class Optimal : public Strategy<int, false>, public MaxRowsBased {
    Optimal(){};
    int col = 0;
public:
    Optimal(const Matrix m, int arg){
        submatrices.push_back(m);
        MaxRowsBased::threshold = arg; // real dumb
    }
    bool has_more_nodes(){
        // the submatrix == -1 part is just in case
        return submatrix == -1 || col < submatrices[submatrix].C;
    }
    Optimal next_node(){
        Optimal result;
        std::vector<Matrix>& subs = result.subs();
        // copy current submatrices
        subs = submatrices;
        // copy intercluster columns
        result.intercluster = intercluster;
        // add id of the removed column
        result.intercluster.insert(subs[submatrix].col_id[col]);
        // remove a column from the current one
        subs[submatrix] = subs[submatrix].removeColumn(col++);
        return result;
    }
    int get_bound(){
        int sum = 0;
        for(auto& m : submatrices)
            sum += m.ones();
        return sum;
    }
};

class Blocking : public Strategy<int, false>, public MaxRowsBased {
    Blocking(){};
    std::vector<int> cols_to_remove;
public:
    Blocking(const Matrix& m, int arg){
        submatrices.push_back(m);
        MaxRowsBased::threshold = arg; // real dumb
    }
    Blocking(const Blocking& other):Strategy(other){
        cols_to_remove = other.cols_to_remove;
    }
    Blocking(Blocking&& other):Strategy(std::move(other)){
        cols_to_remove = std::move(other.cols_to_remove);
    }
    bool has_more_nodes(){
        // the submatrix == -1 part is just in case
        return submatrix == -1 || !cols_to_remove.empty();
    }
    Blocking next_node(){
        Blocking result;
        std::vector<Matrix>& subs = result.subs();
        // copy current submatrices
        subs = submatrices;
        // copy intercluster columns
        result.intercluster = intercluster;
        // add id of the removed column
        result.intercluster.insert(subs[submatrix].col_id[cols_to_remove.back()]);
        // remove a column from the current one
        subs[submatrix] = subs[submatrix].removeColumn(cols_to_remove.back());
        cols_to_remove.pop_back();
        return result;
    }
    void start_branching(int i){
        Strategy::start_branching(i);
        cols_to_remove = blockingMeasure(submatrices[i]);
    }
    int get_bound(){
        int sum = 0;
        for(auto& m : submatrices)
            sum += m.ones();
        return sum;
    }
};

class Void : public Strategy<int, false>, public MaxRowsBased {
    Void(){};
    std::vector<int> cols_to_remove;
public:
    Void(const Matrix& m, int arg){
        submatrices.push_back(m);
        MaxRowsBased::threshold = arg; // real dumb
    }
    Void(const Void& other):Strategy(other){
        cols_to_remove = other.cols_to_remove;
    }
    Void(Void&& other):Strategy(std::move(other)){
        cols_to_remove = std::move(other.cols_to_remove);
    }
    bool has_more_nodes(){
        // the submatrix == -1 part is just in case
        return submatrix == -1 || !cols_to_remove.empty();
    }
    Void next_node(){
        Void result;
        std::vector<Matrix>& subs = result.subs();
        // copy current submatrices
        subs = submatrices;
        // copy intercluster columns
        result.intercluster = intercluster;
        // add id of the removed column
        result.intercluster.insert(subs[submatrix].col_id[cols_to_remove.back()]);
        // remove a column from the current one
        subs[submatrix] = subs[submatrix].removeColumn(cols_to_remove.back());
        cols_to_remove.pop_back();
        return result;
    }
    void start_branching(int i){
        Strategy::start_branching(i);
        cols_to_remove = voidMeasure(submatrices[i]);
    }
    int get_bound(){
        int sum = 0;
        for(auto& m : submatrices)
            sum += m.ones();
        return sum;
    }
};

class Strategy09: public Strategy<int, true>, public CohesionBased {
    Strategy09(){};
    int col = 0;
    int access = 0;
    std::vector<int> rows;
    int bound = 0;
    static std::vector<int> frequencies;
    static int getFreq(int i){
        if(frequencies.size()) return frequencies[i];
        else return 1;
    }
public:
    Strategy09(const Matrix& m, float arg){
        submatrices.push_back(m);
        CohesionBased::threshold = arg; // real dumb
    }
    Strategy09(const Strategy09& other):Strategy(other){
        rows = other.rows;
        col = other.col;
        access = other.access;
        bound = other.bound;
    }
    Strategy09(Strategy09&& other):Strategy(std::move(other)){
        rows = std::move(other.rows);
        col = other.col;
        access = other.access;
        bound = other.bound;
    }
    bool has_more_nodes(){
        return submatrix == -1 || access < rows.size();
    }
    Strategy09 next_node(){
        Strategy09 result;
        std::vector<Matrix>& subs = result.subs();
        std::set<int>& inter = result.intercluster;
        // copy current submatrices
        subs = submatrices;
        // remove an access
        int i = rows[access++];
        subs[submatrix][col][i] = 0;
        // copy intercluster columns
        result.intercluster = intercluster;
        // add id of the removed column if it's not there yet
        // supposedly unnecessary since CI will remove zero columns and rows
        //subs[submatrix] = subs[submatrix].optimize();
        result.bound = bound + getFreq(subs[submatrix].row_id[i]);
        return result;
    }
    void start_branching(int i){
        Strategy::start_branching(i);
        Matrix& sub = submatrices[i];
        rows.clear();
        access = 0;
        col = voidMeasure(sub)[0];
        for(int j = 0; j < sub.R; ++j)
            if(sub[col][j]) rows.push_back(j);
        // this node will not be a final solution so we can 'corrupt' it
        intercluster.insert(sub.col_id[col]);
        //sort(rows.begin(), rows.end(), [&sub](int a, int b){
        //        return getFreq(sub.row_id[a]) > getFreq(sub.row_id[b]);
        //    });
        #ifdef DEBUG
        std::cerr << "Row freqs: ";
        for(int j = 0; j < sub.R; j++) std::cerr << getFreq(sub.row_id[j]) << ' ';
        std::cerr << std::endl;
        std::cerr << "Row order: ";
        for(auto x : rows) std::cerr << sub.row_id[x] + 1 << ' ';
        std::cerr << std::endl;
        #endif
    }
    int get_bound(){
        return bound;
    }
    static void setFreqs(std::vector<int>& freqs){
        frequencies = std::move(freqs);
    }
};

// template-based approach can be abandoned in favor of a virtual base class one
// if deemed more appropriate (probably is)

template<class Node, typename additional> Solution BAB(const Matrix& m, bool return_first, additional arg = additional()){
    std::stack<Node> tree;
    Solution solution;
    ++nodes;
    tree.push(Node {m, arg});
    auto best = Node::init_bound();
    auto bound = best;
    while(!tree.empty()){
        // get the lowest node
        Node& current = tree.top();
        // extract the submatrices
        std::vector<Matrix>& subs = current.subs();
        #ifdef DEBUG
        std::cerr << "Looking at: " << std::endl;
        for(auto& sub : subs)
            std::cerr << sub << std::endl;
        #endif
        // check them, stopping if branching starts
        for(int i = 0; !current.is_branching() && i < subs.size();)
            if(Node::valid_submatrix(subs[i]))
                ++i; // go on if valid
            else {
                // if not, apply the CI algorithm
                std::vector<Matrix> clusters = subs[i].clusters();
                if(clusters.size() > 1){
                    // if the submatrix is decomposable, replace it with its clusters
                    // we are supposed to create a new node, but there's no difference
                    subs.erase(subs.begin() + i);
                    subs.insert(subs.begin() + i, make_move_iterator(clusters.begin()), make_move_iterator(clusters.end()));
                    // CI will be called again, but it shouldn't be a problem
                } else if(clusters.size() == 1){
                    // if the matrix is not decomposable, start branching
                    current.start_branching(i);
                } else goto invalid_node;
            }
        if(current.is_branching()){
            while(current.has_more_nodes()){
                Node child = current.next_node();
                // we do not allow empty submatrices
                if(child.has_empty_rows_cols()){
                    #ifdef DEBUG
                    std::cerr << "solution has one or several empty rows or columns" << std::endl;
                    continue;
                    #endif
                }
                bound = child.get_bound();
                #ifdef DEBUG
                std::cerr << "child bound: " << bound << std::endl;
                #endif
                // skip node if solution will be worse
                if(!Node::is_better(bound, best)){
                    #ifdef DEBUG
                    std::cerr << "solution won't be better, skipping" << std::endl;
                    #endif
                    continue;
                }
                ++nodes;
                tree.push(std::move(child));
                goto done_with_node; // can be replaced, is local enough to be left as is
            }
        } else {
            if(current.has_empty_rows_cols()){
                #ifdef DEBUG
                std::cerr << "solution has one or several empty rows or columns" << std::endl;
                #endif
                goto invalid_node;
            }
            // all good, that's a solution
            if(return_first)
                return std::move(current);
            #ifdef DEBUG
            std::cerr << "solution" << std::endl;
            #endif
            bound = current.get_bound();
            if(Node::is_better(bound, best)){
                // save it if it's really better
                #ifdef DEBUG
                std::cerr << "a better one" << std::endl;
                #endif
                solution = std::move(current);
                best = bound;
            }
        }
        // either no more nodes can be generated
        // or this is a solution
        // move up the tree
        invalid_node:;
        tree.pop();
        done_with_node:;
    }
    return solution;
}
