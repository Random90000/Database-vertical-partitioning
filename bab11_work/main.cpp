#include <iostream>
#include "bab11.hpp"

using namespace std;

Matrix* duplicate(const Matrix* m, int attribute) {
    //find index of attribute in col_id
    int attribute_id = -1;
    for (int i = 0; i < m->R; i++) {
        if (m->row_id[i] == attribute) {
            attribute_id = i;
        }
    }

    //count number of "1" in attribute column
    int count = 0;
    for (int i = 0; i < m->R; i++) {
        if ((*m)[attribute_id][i] != 0) {
            count++;
        }
    }
    //count of new columns
    count--;
    //filling in not duplicated parts and assigning rows/cols id
    Matrix* duplicate_m = new Matrix(m->R, m->C + count);
    cout << attribute_id << " " << count << "\n" << *duplicate_m << "\n";
    for (int i = 0; i < m->R; i++) {
        for (int j = 0; j < attribute_id; j++) {
            (*duplicate_m)[j][i] = (*m)[j][i];
            duplicate_m->col_id[j] = m->col_id[j];
        }
        for (int j = attribute_id; j < attribute_id + count + 1; j++) {
            (*duplicate_m)[j][i] = 0;
            duplicate_m->col_id[j] = attribute;
        }
        for (int j = attribute_id + count + 1; j < m->C + count; j++) {
            (*duplicate_m)[j][i] = (*m)[j - count][i];
        }
        duplicate_m->row_id[i] = m->row_id[i];
    }
    //filling in duplicated parts
    cout << *duplicate_m << "\n";
    for (int i = 0, unit_i = 0; unit_i < count + 1; i++) {
        if ((*m)[attribute_id][i] == 1) {
            (*duplicate_m)[attribute_id + unit_i][i] = 1;
            unit_i++;
            cout << i << " " << unit_i << "\n" << *duplicate_m << "\n";
        }
    }
    return duplicate_m;
}

int main()
{
    int n,m;
    cin >> n >> m;
    Matrix m1(n,m);
    cin >> m1;
    for (int i =0 ;i < n; i++){
        cin >> m1.row_id[i];
    }
    for (int j = 0; j < m;j++){
        cin >> m1.col_id[j];
    }
    cout << *duplicate(&m1, 2);
    return 0;
}
