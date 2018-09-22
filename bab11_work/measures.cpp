#include "measures.hpp"

#define DEBUG

using namespace std;

// returns columns with the highest measure
static vector<int> getMax(vector<int> measures){
    int max = 0;
    for(auto measure : measures)
        if(measure > max) max = measure;
    vector<int> cols;
    for(int i = 0; i < measures.size(); ++i)
        if(measures[i] == max) cols.push_back(i);
    return cols;
}

vector<int> voidMeasure(Matrix m){
    // naive, can it be optimized?
    if(m.C == 1) return {0};
    vector<int> V(m.C);
    vector<bool> R(m.R);
    vector<bool> C(m.C);
    for(int c = 0; c < m.C; ++c){
        R.assign(m.R, false);
        C.assign(m.C, false);
        Matrix::type* column = m[c];
        for(int i = 0; i < m.R; ++i){
            if(column[i]){
                for(int j = 0; j < m.C; ++j){
                    if(j != c && m[j][i]) C[j] = true;
                }
            }
        }
        C[c] = true;
        for(int i = 0; i < m.R; ++i){
            for(int j = 0; j < m.C; ++j)
                if(C[j] && m[j][i] && m[c][i]) R[i] = true;
        }
        V[c] = 0;
        for(int i = 0; i < m.R; ++i){
            if(!R[i]) continue;
            for(int j = 0; j < m.C; ++j)
                if(C[j] && !m[j][i]) ++V[c];
        }
    }
    #ifdef DEBUG
    cerr << "Measures: ";
    for(int i : V) cerr << i << ' ';
    cerr << endl;
    #endif
    return getMax(V);
}

// not exactly as in the article
// we do not divide by (n-1) since it's a constant and
// therefore would make no difference
vector<int> blockingMeasure(Matrix m){
    if(m.C == 1) return {0};
    vector<int> V(m.C);
    for(int c = 0; c < m.C; ++c){
        V[c] = 0;
        for(int i = 0; i < m.R; ++i)
            if(m[c][i] == 1)
                for(int j = 0; j < m.C; ++j)
                    if(m[j][i] == 1) ++V[c];
    }
    #ifdef DEBUG
    cerr << "Measures: ";
    for(int i : V) cerr << i << ' ';
    cerr << endl;
    #endif
    return getMax(V);
}
