#ifndef BAKALARSKAPRACA_ALGORITHMS_H
#define BAKALARSKAPRACA_ALGORITHMS_H

#include <chrono>
#include <cmath>
#include "Matrix.h"

enum laplaceVariant { FULL_LAPLACE_EXPANSION, LAPLACE_RULE_OF_SARRUS, LAPLACE_GAUSS_ELIMINATION };

class Algorithms {
public:
    auto getCurrentTime() {
        return std::chrono::system_clock::now();
    }

    template<typename T>
    T gaussEliminationMethod(Matrix<T>& matrix, bool characterOutput) {
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
                result = 0;
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
        }

        for (int i = 0; i < pivots.size(); ++i) {
            result *= pivots[i];
        }
        auto end = getCurrentTime();
        std::chrono::duration<double> elapsedTime = end - start;

        if (characterOutput) {
            std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
            std::cout << "Calculation duration of the Gauss Elimination: " << elapsedTime << std::endl;
        }
        return result;
    }

    template<typename T>
    T leibnizMethod(Matrix<T>& matrix, bool characterOutput) {
        auto start = getCurrentTime();
        T result = 0;
        int numberOfSwaps;
        T product;
        std::vector<int> columns;

        for (int i = 0; i < matrix.getSize(); ++i) {
            columns.push_back(i);
        }

        do {
            product = 1;
            for (int i = 0; i < matrix.getSize(); ++i) {
                product *= matrix[i][columns[i]];
            }
            numberOfSwaps = 0;
            for (int i = 0; i < columns.size(); ++i) {
                for (int j = i + 1; j < columns.size(); ++j) {
                    if (columns[i] > columns[j]) {
                        numberOfSwaps++;
                    }
                }
            }
            if (numberOfSwaps % 2 != 0) {
                product *= (-1);
            }
            result += product;
        } while (std::next_permutation(columns.begin(), columns.end()));

        auto end = getCurrentTime();

        std::chrono::duration<double> elapsedTime = end - start;

        if (characterOutput) {
            std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
            std::cout << "Calculation duration of the Leibniz method: " << elapsedTime << std::endl;
        }
        return result;
    }

    template<typename T>
    void laplaceMethod(Matrix<T>& matrix, laplaceVariant variant) {
        auto start = getCurrentTime();
        T result = Algorithms::laplaceExpansion(matrix, variant);
        auto end = getCurrentTime();

        std::chrono::duration<double> elapsedTime = end - start;
        std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
        switch (variant) {
            case FULL_LAPLACE_EXPANSION: std::cout << "Calculation duration of the Full Laplace expansion: " << elapsedTime << std::endl; break;
            case LAPLACE_RULE_OF_SARRUS: std::cout << "Calculation duration of the Laplace expansion with Rule of Sarrus: " << elapsedTime << std::endl; break;
            case LAPLACE_GAUSS_ELIMINATION: std::cout << "Calculation duration of the Laplace expansion with Gauss Elimination: " << elapsedTime << std::endl; break;
            default: std::cerr << "Invalid variant of the Laplace Expansion." << std::endl; break;
        }
    }

    template<typename T>
    T laplaceExpansion(Matrix<T>& matrix, laplaceVariant variant) {
        T result = 0;
        int maxNumberOfZerosInColumn = 0;
        int maxNumberOfZerosInRow = 0;
        int indexOfRow = 0;
        int indexOfColumn = 0;


        for (int i = 0; i < matrix.getSize(); ++i) {
            int zerosInRow = 0;
            for (int j = 0; j < matrix.getSize(); ++j) {
                if (matrix[i][j] == 0) {
                    zerosInRow++;
                }
            }
            if (zerosInRow > maxNumberOfZerosInRow) {
                maxNumberOfZerosInRow = zerosInRow;
                indexOfRow = i;
            }
        }

        for (int j = 0; j < matrix.getSize(); ++j) {
            int zerosInColumn = 0;
            for (int i = 0; i < matrix.getSize(); ++i) {
                if (matrix[i][j] == 0) {
                    zerosInColumn++;
                }
                if (zerosInColumn > maxNumberOfZerosInColumn) {
                    maxNumberOfZerosInColumn = zerosInColumn;
                    indexOfColumn = j;
                }
            }
        }

        if (maxNumberOfZerosInRow >= maxNumberOfZerosInColumn) {
            for (int i = 0; i < matrix.getSize(); ++i) {
                Matrix<T> newMatrix(matrix.getSize() - 1);
                T pivot = matrix[indexOfRow][i];

                int newRow = 0;
                for (int j = 0; j < matrix.getSize(); ++j) {
                    if (j != indexOfRow) {
                        int newColumn = 0;
                        for (int k = 0; k < matrix.getSize(); ++k) {
                            if (k != i) {
                                newMatrix[newRow][newColumn] = matrix[j][k];
                                newColumn++;
                            }
                        }
                        newRow++;
                    }
                }
                if (newMatrix.getSize() > 1) {
                    if (pivot != 0) {
                        if (newMatrix.getSize() == 4) {
                            switch (variant) {
                                case FULL_LAPLACE_EXPANSION: result += pivot * pow(-1, indexOfRow + i) * laplaceExpansion(newMatrix, variant); break;
                                case LAPLACE_RULE_OF_SARRUS: result += pivot * pow(-1, indexOfRow + i) * leibnizMethod(newMatrix, false); break;
                                case LAPLACE_GAUSS_ELIMINATION: result += pivot * pow(-1, indexOfRow + i) * gaussEliminationMethod(newMatrix, false); break;
                                default: std::cerr << "Invalid variant of the Laplace Expansion" << std::endl; return -1;
                            }
                        } else {
                            result += pivot * pow(-1, indexOfRow + i) * laplaceExpansion(newMatrix, variant);
                        }
                    }
                } else {
                    result += pivot * pow(-1, indexOfRow + i) * newMatrix[0][0];
                }
            }
        } else {
            for (int i = 0; i < matrix.getSize(); ++i) {
                Matrix<T> newMatrix(matrix.getSize() - 1);
                T pivot = matrix[i][indexOfColumn];

                int newRow = 0;
                for (int j = 0; j < matrix.getSize(); ++j) {
                    if (j != i) {
                        int newColumn = 0;
                        for (int k = 0; k < matrix.getSize(); ++k) {
                            if (k != indexOfColumn) {
                                newMatrix[newRow][newColumn] = matrix[j][k];
                                newColumn++;
                            }
                        }
                        newRow++;
                    }
                }
                if (newMatrix.getSize() > 1) {
                    if (pivot != 0) {
                        if (newMatrix.getSize() == 4) {
                            switch (variant) {
                                case FULL_LAPLACE_EXPANSION: result += pivot * pow(-1, indexOfColumn + i) * laplaceExpansion(newMatrix, variant); break;
                                case LAPLACE_RULE_OF_SARRUS: result += pivot * pow(-1, indexOfColumn + i) * leibnizMethod(newMatrix, false); break;
                                case LAPLACE_GAUSS_ELIMINATION: result += pivot * pow(-1, indexOfColumn + i) * gaussEliminationMethod(newMatrix, false); break;
                                default: std::cerr << "Invalid variant of the Laplace Expansion" << std::endl; return -1;
                            }
                        } else {
                            result += pivot * pow(-1, indexOfColumn + i) * laplaceExpansion(newMatrix, variant);
                        }
                    }
                } else {
                    result += pivot * pow(-1, indexOfColumn + i) * newMatrix[0][0];
                }
            }
        }
        return result;
    }
};


#endif
