#ifndef BAKALARSKAPRACA_MATRIX_H
#define BAKALARSKAPRACA_MATRIX_H

#include <fstream>
#include <iostream>
#include <vector>

template<class T>
class Matrix {
private:
    std::vector<std::vector<T>> matrix_;
    int size_;
public:
    Matrix(int size) : size_(size) {
        for (int i = 0; i < size_; ++i) {
            std::vector<T> row(size_);
            matrix_.push_back(row);
        }
    };

    void setValue(int row, int column, T value) {
        matrix_[row][column] = value;
    };

    void print() {
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                std::cout << matrix_[i][j] << " ";
            }
            std::cout <<std::endl;
        }
        std::cout << std::endl;
    };

    int getSize() {
        return size_;
    };

    std::vector<T>& operator[](int index) {
        return matrix_[index];
    }
};

#endif
