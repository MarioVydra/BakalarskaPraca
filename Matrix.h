#ifndef BAKALARSKAPRACA_MATRIX_H
#define BAKALARSKAPRACA_MATRIX_H

#include <fstream>
#include <iostream>
#include <vector>
#include "Generator.h"

template<class T>
class Matrix {
private:
    int size_;
    T** matrix_;
public:
    Matrix(int size) : size_(size) {
        matrix_ = new T*[size_];
        for (int i = 0; i < size_; ++i) {
            matrix_[i] = new T[size_];
        }

        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                matrix_[i][j] = 0;
            }
        }
    };

    ~Matrix() {
        for (int i = 0; i < size_; ++i) {
            delete[] matrix_[i];
        }
        delete[] matrix_;
    }

    void setValue(int row, int column, T value) {
        matrix_[row][column] = value;
    };

    template<typename G>
    void generateValues(Generator<G>& generator) {
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                matrix_[i][j] = generator.generate();
            }
        }
    }

    void print() {
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                std::cout << matrix_[i][j] << " ";
            }
            std::cout <<std::endl;
        }
        std::cout << std::endl;
    };

    int countZeros() {
        int zeros = 0;
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                if (matrix_[i][j] == 0) {
                    zeros++;
                }
            }
        }
        return zeros;
    };

    int getSize() {
        return size_;
    };

   T*& operator[](int index) {
        return matrix_[index];
   }
};

#endif
