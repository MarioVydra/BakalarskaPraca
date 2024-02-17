#ifndef BAKALARSKAPRACA_ALGORITHMS_H
#define BAKALARSKAPRACA_ALGORITHMS_H

#include <chrono>
#include <cmath>
#include "Matrix.h"


class Algorithms {
public:
    auto getCurrentTime() {
        return std::chrono::system_clock::now();
    }

    template<typename T>
    void gaussEliminationMethod(Matrix<T>& matrix) {
        matrix.print();

        auto start = getCurrentTime();
        std::vector<T> pivots;
        T result = 1;
        T delta = 1e-10;

        for (int i = 0; i < matrix.getSize(); ++i) {
            T pivot = 0;
            /*for (int j = 0; j < matrix.getSize(); ++j) {
                if (matrix[i][j] != 0) {
                    pivot = matrix[i][j];
                    column = j;
                    pivots.push_back(pivot);
                    break;
                }
            }*/
            for (int j = i; j < matrix.getSize(); ++j) {
                if (matrix[j][i] != 0) {
                    if (j != i) {
                        matrix[i].swap(matrix[j]);
                        result *= (-1);
                    }
                    pivot = matrix[i][i];
                    pivots.push_back(matrix[i][i]);
                    break;
                }
            }

            if (pivot == 0) {
                std::cout << "The determinant of the matrix is equal to: 0" << std::endl;
                break;
            }

            /*for (int j = column; j < matrix.getSize(); ++j) {
                matrix[i][j] *= pow(pivot, -1);
            }*/

            for (int j = i + 1; j < matrix.getSize(); ++j) {
                T number = -matrix[j][i];
                for (int k = i; k < matrix.getSize(); ++k) {
                    matrix[j][k] += matrix[i][k] * (number / pivot);
                    //matrix[j][k] += matrix[i][k] * number;
                    if ((matrix[j][k] < delta && matrix[j][k] > 0) || (matrix[j][k] > -delta && matrix[j][k] < 0)) {
                        matrix[j][k] = 0;
                    }
                }
            }
            //matrix.print();
        }

        for (int i = 0; i < pivots.size(); ++i) {
            result *= pivots[i];
        }
        auto end = getCurrentTime();
        std::chrono::duration<double> elapsedTime = end - start;

        matrix.print();
        std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
        std::cout << "Calculation duration: " << elapsedTime << std::endl;
    }

    template<typename T>
    void leibnizMethod(Matrix<T>& matrix) {
        auto start = getCurrentTime();

        auto end = getCurrentTime();

        std::chrono::duration<double> elapsedTime = end - start;
        std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
        std::cout << "Calculation duration: " << elapsedTime << std::endl;
    }
};


#endif
