#include "tst_bab11.h"

#include <gtest/gtest.h>

#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <fstream>
#include <iostream>

#include "../bab11_work/measures.cpp"
#include "../bab11_work/matrix.cpp"
#include "../bab11_work/bab11.cpp"

using namespace testing;

void compare_matrices(Matrix* m1, Matrix* m2) {
    ASSERT_EQ(m1->R, m2->R);
    ASSERT_EQ(m1->C, m2->C);
    for (int i = 0 ; i < m1->R; i++) {
        ASSERT_EQ(m1->row_id[i], m2->row_id[i]);
    }
    for (int i = 0 ; i < m1->C; i++) {
        ASSERT_EQ(m1->col_id[i], m2->col_id[i]);
    }
    for (int i = 0 ; i < m1->R * m1->C; i++) {
        ASSERT_EQ(m1->elements[i], m2->elements[i]);
    }
}

/* input_data format:
 * rows cols
 * input_matrix
 *
 * mr
 *
 * mc
 *
 * rows cols
 * correct_matrix
 *
 * correct_matrix_row_id
 * correct_matrix_col_id
 */

void read_and_run_select_minor_test(string test_dir) {
    ifstream in(test_dir);
    int input_matrix_R    = 0;
    int input_matrix_C    = 0;
    int correct_matrix_R  = 0;
    int correct_matrix_C  = 0;
    bool input_vector_val = false;

    in >> input_matrix_R >> input_matrix_C;
    Matrix input_matrix(input_matrix_R, input_matrix_C);
    in >> input_matrix;
    std::vector<bool> mr(static_cast<ulong>(input_matrix.R), false);
    for (int i = 0; i < input_matrix.R; i++) {
        in >> input_vector_val;
        mr[static_cast<ulong>(i)] = input_vector_val;
    }
    std::vector<bool> mc(static_cast<ulong>(input_matrix.C), false);
    for (int i = 0; i < input_matrix.C; i++) {
        in >> input_vector_val;
        mc[static_cast<ulong>(i)] = input_vector_val;
    }

    Matrix* output_matrix = select_minor(&input_matrix, &mr, &mc);

    in >> correct_matrix_R >> correct_matrix_C;
    Matrix correct_matrix(correct_matrix_R, correct_matrix_C);
    in >> correct_matrix;

    for (int i = 0; i < correct_matrix.R; i++) {
        in >> correct_matrix.row_id[i];
    }
    for (int i = 0; i < correct_matrix.C; i++) {
        in >> correct_matrix.col_id[i];
    }
    compare_matrices(&correct_matrix, output_matrix);
    delete output_matrix;
}

TEST(select_minor, 1) {
    read_and_run_select_minor_test("../test_1");
}

TEST(select_minor, 2) {
    read_and_run_select_minor_test("../test_2");
}

TEST(select_minor, 3) {
    read_and_run_select_minor_test("../test_3");
}

TEST(select_minor, 4) {
    read_and_run_select_minor_test("../test_4");
}

/* input_data format:
 * rows cols
 * input_matrix
 *
 * rows cols
 * fst_correct_matrix
 *
 * fst_mr
 *
 * fst_mc
 *
 * rows cols
 * snd_correct_matrix
 *
 * snd_mr
 *
 * snd_mc
 */

void read_and_run_binary_split_test(string test_dir) {
    ifstream in(test_dir);
    int input_matrix_R       = 0;
    int input_matrix_C       = 0;
    int fst_correct_matrix_R = 0;
    int fst_correct_matrix_C = 0;
    int snd_correct_matrix_R = 0;
    int snd_correct_matrix_C = 0;

    in >> input_matrix_R >> input_matrix_C;
    Matrix input_matrix(input_matrix_R, input_matrix_C);
    in >> input_matrix;

    std::pair<Matrix*, Matrix*>* splitted_matrices = binary_split(&input_matrix);

    in >> fst_correct_matrix_R >> fst_correct_matrix_C;
    Matrix fst_correct_matrix(fst_correct_matrix_R, fst_correct_matrix_C);

    in >> fst_correct_matrix;

    for (int i = 0; i < fst_correct_matrix_R; i++) {
        in >> fst_correct_matrix.row_id[i];
    }

    for (int i = 0; i < fst_correct_matrix_C; i++) {
        in >> fst_correct_matrix.col_id[i];
    }

    in >> snd_correct_matrix_R >> snd_correct_matrix_C;
    Matrix snd_correct_matrix(snd_correct_matrix_R, snd_correct_matrix_C);

    in >> snd_correct_matrix;

    for (int i = 0; i < snd_correct_matrix_R; i++) {
        in >> snd_correct_matrix.row_id[i];
    }
    for (int i = 0; i < snd_correct_matrix_C; i++) {
        in >> snd_correct_matrix.col_id[i];
    }
    compare_matrices(&fst_correct_matrix, splitted_matrices->first);
    compare_matrices(&snd_correct_matrix, splitted_matrices->second);
    delete splitted_matrices->first;
    delete splitted_matrices->second;
    delete splitted_matrices;
}

TEST(binary_split, 5) {
    read_and_run_binary_split_test("../test_5");
}

TEST(binary_split, 6) {
    read_and_run_binary_split_test("../test_6");
}

/* input_data format:
 * rows cols
 * input_matrix
 *
 * vector_size
 * matrix_clusterized_vector {
 *  matrix_num_i
 *  matrix_num_i_mr
 *  matrix_num_i_mc
 * }
 */

void read_and_run_cluster_identification_test(string test_dir) {
    ifstream in(test_dir);
    int input_matrix_R = 0;
    int input_matrix_C = 0;
    int clusters_size  = 0;

    in >> input_matrix_R >> input_matrix_C;
    Matrix input_matrix(input_matrix_R, input_matrix_C);
    in >> input_matrix;

    vector<Matrix*>* output_clusters = cluster_identification(&input_matrix);

    in >> clusters_size;
    ASSERT_EQ(clusters_size, output_clusters->size());
    vector<Matrix> clusters;
    for (int i = 0; i < clusters_size; i++) {
        int current_matrix_R = 0;
        int current_matrix_C = 0;
        in >> current_matrix_R >> current_matrix_C;
        Matrix current_matrix(current_matrix_R, current_matrix_C);
        in >> current_matrix;
        for (int i = 0; i < current_matrix.R; i++) {
            in >> current_matrix.row_id[i];
        }

        for (int i = 0; i < current_matrix.C; i++) {
            in >> current_matrix.col_id[i];
        }
        compare_matrices(&current_matrix, (*output_clusters)[static_cast<ulong>(i)]);
    }
    for (auto i : *output_clusters) {
        delete i;
    }
    delete output_clusters;
}

TEST(cluster_identification, 7) {
    read_and_run_cluster_identification_test("../test_7");
}

TEST(cluster_identification, 8) {
    read_and_run_cluster_identification_test("../test_8");
}

/* input_data format:
 * m1_rows m1_cols
 * matrix_m1
 *
 * m1_row_id
 *
 * m1_col_id
 *
 *
 * m2_rows m2_cols
 * matrix_m2
 *
 * m2_row_id
 *
 * m2_col_id
 *
 *
 * output_matrix_rows output_matrix_cols
 * output_matrix
 *
 * output_matrix_row_id
 *
 * output_matrix_col_id
 */

void read_and_run_merge_clusters_test(string test_dir) {
    ifstream in(test_dir);
    int matrix_R = 0;
    int matrix_C = 0;

    in >> matrix_R >> matrix_C;
    Matrix m1(matrix_R, matrix_C);
    in >> m1;
    for (int i = 0; i < m1.R; i++) {
        in >> m1.row_id[i];
    }
    for (int i = 0; i < m1.C; i++) {
        in >> m1.col_id[i];
    }

    in >> matrix_R >> matrix_C;
    Matrix m2(matrix_R, matrix_C);
    in >> m2;
    for (int i = 0; i < m2.R; i++) {
        in >> m2.row_id[i];
    }
    for (int i = 0; i < m2.C; i++) {
        in >> m2.col_id[i];
    }

    in >> matrix_R >> matrix_C;
    Matrix output_matrix(matrix_R, matrix_C);
    in >> output_matrix;
    for (int i = 0; i < output_matrix.R; i++) {
        in >> output_matrix.row_id[i];
    }
    for (int i = 0; i < output_matrix.C; i++) {
        in >> output_matrix.col_id[i];
    }

    Matrix* merged = merge_clusters(&m1, &m2);
    compare_matrices(&output_matrix, merged);
    delete merged;
}

TEST(merge_clusters, 9) {
    read_and_run_merge_clusters_test("../test_9");
}

TEST(merge_clusters, 10) {
    read_and_run_merge_clusters_test("../test_10");
}

TEST(merge_clusters, 11) {
    read_and_run_merge_clusters_test("../test_11");
}

/*
TEST(merge_clusters, 12) {
    read_and_run_merge_clusters_test("../test_12");
}
*/

/* input_data format:
 * rows cols
 * input_matrix
 *
 * input_matrix_row_id
 *
 * input_matrix_col_id
 *
 * attribute
 *
 * rows cols
 * correct_matrix
 *
 * correct_matrix_row_id
 *
 * correct_matrix_col_id
 */

void read_and_run_duplicate_test(string test_dir) {
    ifstream in(test_dir);
    int input_matrix_R = 0;
    int input_matrix_C = 0;

    in >> input_matrix_R >> input_matrix_C;
    Matrix input_matrix(input_matrix_R, input_matrix_C);
    in >> input_matrix;

    for (int i = 0; i < input_matrix_R; i++) {
        in >> input_matrix.row_id[i];
    }

    for (int i = 0; i < input_matrix_C; i++) {
        in >> input_matrix.col_id[i];
    }

    int attribute        = 0;
    int correct_matrix_R = 0;
    int correct_matrix_C = 0;

    in >> attribute;
    in >> correct_matrix_R >> correct_matrix_C;
    Matrix correct_matrix(correct_matrix_R, correct_matrix_C);
    in >> input_matrix;

    for (int i = 0; i < correct_matrix_R; i++) {
        in >> input_matrix.row_id[i];
    }

    for (int i = 0; i < correct_matrix_C; i++) {
        in >> input_matrix.col_id[i];
    }

    Matrix* duplicated = duplicate(&input_matrix, attribute);
    compare_matrices(&correct_matrix, duplicated);
    delete duplicated;
}

TEST(duplicate, 13) {
    read_and_run_duplicate_test("../test_13");
}

TEST(duplicate, 14) {
    read_and_run_duplicate_test("../test_14");
}

TEST(duplicate, 15) {
    read_and_run_duplicate_test("../test_15");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
