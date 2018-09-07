#include <algorithm>
#include <string.h>
#include <cmath>

#include "matrix.hpp"

using namespace std;

Matrix::Matrix(int rows, int cols){
    R = rows;
    C = cols;

    row_id = new int[R];
    col_id = new int[C];
    elements = new type[R * C];
    for(int i = 0; i < R; ++i) row_id[i] = i;
    for(int i = 0; i < C; ++i) col_id[i] = i;
};

Matrix::Matrix(Matrix&& other) noexcept {
    memcpy(this, &other, sizeof(Matrix));
    memset(&other, 0, sizeof(Matrix));
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    this->~Matrix();
    memcpy(this, &other, sizeof(Matrix));
    memset(&other, 0, sizeof(Matrix));
    return *this;
}

Matrix::Matrix(const Matrix& other) noexcept {
    R = other.R; C = other.C;

    row_id = new int[R];
    col_id = new int[C];
    elements = new type[R*C];

    memcpy(row_id, other.row_id, sizeof(int) * R);
    memcpy(col_id, other.col_id, sizeof(int) * C);
    memcpy(elements, other.elements, sizeof(type) * R * C);
}

Matrix& Matrix::operator=(const Matrix& other) noexcept {
    this->~Matrix();
    R = other.R; C = other.C;

    row_id = new int[R];
    col_id = new int[C];
    elements = new type[R*C];

    memcpy(row_id, other.row_id, sizeof(int) * R);
    memcpy(col_id, other.col_id, sizeof(int) * C);
    memcpy(elements, other.elements, sizeof(type) * R * C);
    return *this;
}

istream& operator>>(istream& in, const Matrix& m){
    int t;
    int R = m.R;
    int C = m.C;
    Matrix::type* elements = m.elements;

    for(int i = 0; i < R; ++i) m.row_id[i] = i;
    for(int i = 0; i < C; ++i) m.col_id[i] = i;

    for(int r = 0; r < R; ++r)
        for(int c = 0; c < C; ++c){
            in >> t;
            elements[c * R + r] = t;
        }
    return in;
}

static int max(int a, int b){ return a > b ? a : b; }
static void pad(int n, ostream& out){ for(int i = 0; i < n; ++i) out << ' '; }
static int num_digits(int a){
    if(a < 0) return num_digits(-a) + 1; // not sure if needed, but nevertheless
    return floor(log10(a ? a : 1));
}

ostream& operator<<(ostream& out, const Matrix& m){
    int t;
    int R = m.R;
    int C = m.C;
    Matrix::type* elements = m.elements;

    // unnecessary pretty-printing
    // to be removed if anything goes awry

    // pad before column header
    int width = 0;
    for(int i = 0; i < R; ++i)
        width = max(width, m.row_id[i]);
    width = num_digits(width) + 2;

    // print column header and remember the padding between columns
    int* padding = new int[C];
    pad(width, out);
    for(int i = 0; i < C; ++i){
        int id = m.col_id[i] + 1;
        out << ' ' << id;
        padding[i] = 1 + num_digits(id);
    }
    out << endl;

    for(int r = 0; r < R; ++r){
        int id = m.row_id[r] + 1;
        out << id << ':';
        pad(width - (num_digits(id) + 2), out);
        for(int c = 0; c < C; ++c){
            t = elements[c * R + r];
            pad(padding[c], out);
            out << t;
        }
        out << endl;
    }
    delete [] padding;
    return out;
}

bool Matrix::zeroColumn(int c) const {
    type* e = elements + c * R;
    for(int i = 0; i < R; ++i)
        if(*e++) return false;
    return true;
}

bool Matrix::zeroRow(int r) const {
    type* e = elements + r - R;
    for(int i = 0; i < C; ++i)
        if(*(e += R)) return false;
    return true;
}

int Matrix::ones() const {
    size_t size = R*C;
    type* elem = elements;
    int res = 0;
    while(size--)
        if(*(elem++) == 1) ++res;
    return res;
}

float Matrix::cohesion() const {
    return (float)ones() / (R * C);
}

Matrix Matrix::removeColumn(int c) const {
    vector<int> rows;
    vector<int> cols;
    cols.reserve(C-1);
    bool zero;
    for(int i = 0; i < R; ++i){
        zero = true;
        for(int j = 0; j < C; ++j)
            if(j != c && (*this)[j][i] == 1) zero = false;
        if(!zero) rows.push_back(i);
    }
    for(int i = 0; i < C; ++i) if(i != c) cols.push_back(i);

    return submatrix(rows, cols);
}

Matrix Matrix::submatrix(vector<int> rows, vector<int> cols) const {
    sort(rows.begin(), rows.end());
    sort(cols.begin(), cols.end());
    Matrix temp(rows.size(), cols.size());

    for(int i = 0; i < temp.R; ++i) temp.row_id[i] = row_id[rows[i]];
    for(int i = 0; i < temp.C; ++i) temp.col_id[i] = col_id[cols[i]];

    for(int c = 0; c < temp.C; ++c)
        for(int r = 0; r < temp.R; ++r)
            temp[c][r] = (*this)[cols[c]][rows[r]];

    return temp;
}

Matrix Matrix::optimize(){
    vector<int> rows;
    vector<int> cols;
    for(int i = 0; i < R; ++i)
        if(!zeroRow(i))    rows.push_back(i);
    for(int i = 0; i < C; ++i)
        if(!zeroColumn(i)) cols.push_back(i);
    return submatrix(rows, cols);
}

// http://staffweb.hkbu.edu.hk/vwschow/KC-696.pdf
vector<Matrix> Matrix::clusters() const {
    vector<Matrix> clusters;

    vector<int> rows, cols;
    rows.reserve(R);
    cols.reserve(C);
    int r = 0, c = 0;
    bool row = false;
    vector<bool> row_used(R, false);
    vector<bool> col_used(C, false);

    // choose a row for the first horizontal line
    first_line:
    for(int i = 0; i < R; ++i)
        if(!row_used[i]){
            row_used[i] = true;
            if(!zeroRow(i)){
                rows.push_back(i);
                break;
            }
        }

    horizontal:
    if(r < rows.size()){
        for(int i = 0; i < C; ++i)
            if(!col_used[i] && (*this)[i][rows[r]]){
                cols.push_back(i);
                col_used[i] = true;
            }
        ++r;
    }

    vertical:
    if(c < cols.size()){
        for(int i = 0; i < R; ++i)
            if(!row_used[i] && (*this)[cols[c]][i]){
                rows.push_back(i);
                row_used[i] = true;
            }
        ++c;
    }


    // draw more lines if possible
    if(r != rows.size()) goto horizontal;
    if(c != cols.size()) goto vertical;

    // nothing drawn - nothing found
    if(rows.size() == 0 && cols.size() == 0) return clusters;

    // save the cluster and reset
    clusters.push_back(submatrix(rows, cols));
    rows.resize(0); r = 0;
    cols.resize(0); c = 0;

    for(int i = 0; i < R; ++i)
        if(!row_used[i]) goto first_line;

    // can be avoided by making the first line vertical
    sort(clusters.begin(), clusters.end(), [](Matrix m1, Matrix m2){
            return m1.col_id[0] < m2.col_id[0];
        });
    return clusters;
}
