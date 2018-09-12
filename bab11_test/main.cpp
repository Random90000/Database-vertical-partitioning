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

void compare_solutions(Solution s1, Solution s2) {
    ASSERT_EQ(s1.clusters.size(), s2.clusters.size());
    for (size_t i = 0; i < s1.clusters.size(); i++) {
        compare_matrices(&s1.clusters[i], &s2.clusters[i]);
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

TEST(select_minor, 1_1) {
    read_and_run_select_minor_test("../test_1_1");
}

TEST(select_minor, 1_2) {
    read_and_run_select_minor_test("../test_1_2");
}

TEST(select_minor, 1_3) {
    read_and_run_select_minor_test("../test_1_3");
}

TEST(select_minor, 1_4) {
    read_and_run_select_minor_test("../test_1_4");
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

TEST(binary_split, 2_1) {
    read_and_run_binary_split_test("../test_2_1");
}

TEST(binary_split, 2_2) {
    read_and_run_binary_split_test("../test_2_2");
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

TEST(cluster_identification, 3_1) {
    read_and_run_cluster_identification_test("../test_3_1");
}

TEST(cluster_identification, 3_2) {
    read_and_run_cluster_identification_test("../test_3_2");
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

TEST(merge_clusters, 4_1) {
    read_and_run_merge_clusters_test("../test_4_1");
}

TEST(merge_clusters, 4_2) {
    read_and_run_merge_clusters_test("../test_4_2");
}

TEST(merge_clusters, 4_3) {
    read_and_run_merge_clusters_test("../test_4_3");
}

TEST(merge_clusters, 4_4) {
    read_and_run_merge_clusters_test("../test_4_4");
}

/*
TEST(merge_clusters, 4_5) {
    read_and_run_merge_clusters_test("../test_4_5");
}
*/

TEST(merge_clusters, 4_6) {
    read_and_run_merge_clusters_test("../test_4_6");
}

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
    in >> correct_matrix;

    for (int i = 0; i < correct_matrix_R; i++) {
        in >> correct_matrix.row_id[i];
    }

    for (int i = 0; i < correct_matrix_C; i++) {
        in >> correct_matrix.col_id[i];
    }
    Matrix* duplicated = duplicate(&input_matrix, attribute);
    compare_matrices(&correct_matrix, duplicated);
    delete duplicated;
}

TEST(duplicate, 5_1) {
    read_and_run_duplicate_test("../test_5_1");
}

TEST(duplicate, 5_2) {
    read_and_run_duplicate_test("../test_5_2");
}

/* input_data format:
 * number_of_clusters {
 *  matrix_i_rows matrix_i_cols
 *  matrix_i
 *
 *  matrix_i_row_id
 *
 *  matrix_i_col_id
 *
 *  node_i_lower_bound
 *
 *  number_of_duplicated_id
 *  matrix_i_duplicated_id
 * }
 *
 * number_of_solution_clusters {
 *  matrix_i_rows matrix_i_cols
 *  matrix_i
 *
 *  matrix_i_row_id
 *
 *  matrix_i_col_id
 * }
 */

void read_and_run_merge_test(string test_dir) {
    ifstream in(test_dir);
    std::vector<BabNode*> clusters(0);
    int number_of_clusters             = 0;
    int current_matrix_r               = 0;
    int current_matrix_c               = 0;
    int number_of_duplicated           = 0;
    float current_node_low             = 0.;
    std::set<int>* current_duplicated  = new set<int>;
    size_t number_of_solution_clusters = 0;
    int correct_solution_matrix_r      = 0;
    int correct_solution_matrix_c      = 0;

    in >> number_of_clusters;
    for (int i = 0; i < number_of_clusters; i++) {
        in >> current_matrix_r >> current_matrix_c;
        Matrix* current_matrix = nullptr;
        current_matrix         = new Matrix(current_matrix_r, current_matrix_c);
        in >> *current_matrix;
        for (int j = 0; j < current_matrix_r; j++) {
            in >> current_matrix->row_id[j];
        }
        for (int j = 0; j < current_matrix_c; j++) {
            in >> current_matrix->col_id[j];
        }
        in >> current_node_low;
        in >> number_of_duplicated;
        for (int j = 0; j < number_of_duplicated; j++) {
            int current_duplicated_element = 0;
            in >> current_duplicated_element;
            current_duplicated->insert(current_duplicated_element);
        }
        clusters.push_back(new BabNode(current_matrix, current_node_low, current_duplicated));
        delete current_matrix;
    }
    std::cout << "read 1\n" << flush;
    in >> number_of_solution_clusters;
    std::vector<Matrix> correct_solution_clusters;
    for (size_t i = 0; i < number_of_solution_clusters; i++) {
        in >> correct_solution_matrix_r >> correct_solution_matrix_c;
        correct_solution_clusters.push_back(Matrix(correct_solution_matrix_r, correct_solution_matrix_c));
        in >> correct_solution_clusters[i];
        for (int j = 0; j < correct_solution_matrix_r; j++) {
            in >> correct_solution_clusters[i].row_id[j];
        }
        for (int j = 0; j < correct_solution_matrix_c; j++) {
            in >> correct_solution_clusters[i].col_id[j];
        }
    }
    std::cout << "read 2\n" << flush;
    /*
    for (auto t : merge(&clusters).clusters) {
        cout << t << "\n";
    }
    */
    compare_solutions(Solution(correct_solution_clusters), merge(&clusters));
    for (auto c : clusters) {
        delete c;
    }
    delete current_duplicated;
}

TEST(merge, 6_1) {
    read_and_run_merge_test("../test_6_1");
}

TEST(merge, 6_2) {
    read_and_run_merge_test("../test_6_2");
}

TEST(merge, 6_3) {
    read_and_run_merge_test("../test_6_3");
}

TEST(merge, 6_4) {
    read_and_run_merge_test("../test_6_4");
}

TEST(merge, 6_5) {
    read_and_run_merge_test("../test_6_5");
}

TEST(merge, 6_6) {
    read_and_run_merge_test("../test_6_6");
}
/*
TEST(merge, 6_7) {
    read_and_run_merge_test("../test_6_7");
}
*/
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
