#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>

struct Matrix {
    typedef char type;

    int R, C;
    int* row_id;
    int* col_id;
    type* elements;

    Matrix(int rows, int columns);

    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    Matrix(const Matrix&) noexcept;
    Matrix& operator=(const Matrix&) noexcept;

    ~Matrix(){

        //std::cout << "Matrix: destructor " << this->R << " "
        //          << " " << this->C << "\n" << this << "\n"
        //          << *this << "\n";

        delete [] elements; 
        delete [] row_id;
        delete [] col_id;
    }

    // column-major order 
    type* operator[](int index){ return elements + index * R; }
    const type* operator[](int index) const { return elements + index * R; }

    friend std::istream& operator>>(std::istream& in, const Matrix& m);
    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);

    bool zeroColumn(int) const;
    bool zeroRow(int) const;

    int ones() const;
    float cohesion() const;

    Matrix removeColumn(int) const;
    Matrix submatrix(std::vector<int> rows, std::vector<int> cols) const;
    Matrix optimize(); // remove zero rows and columns

    std::vector<Matrix> clusters() const;
};

#endif
