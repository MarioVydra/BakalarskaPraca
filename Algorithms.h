#ifndef BAKALARSKAPRACA_ALGORITHMS_H
#define BAKALARSKAPRACA_ALGORITHMS_H

#include <chrono>
#include <cmath>
#include "Matrix.h"

enum laplaceVariant { FULL_LAPLACE_EXPANSION, LAPLACE_RULE_OF_SARRUS };

template<class T>
class Algorithms {
public:
    auto getCurrentTime() {
        return std::chrono::system_clock::now();
    }

    std::chrono::duration<double> gaussEliminationMethod(Matrix<T>& matrix, bool characterOutput) {
        auto start = getCurrentTime();
        std::vector<T> pivots;
        int matrixSize = matrix.getSize();
        T pivot;
        T result = 1;
        T number;
        T delta = 1e-10;

        for (int i = 0; i < matrixSize; ++i) {
            pivot = 0;
            for (int j = i; j < matrixSize; ++j) {
                if (matrix[j][i] != 0) {
                    if (j != i) {
                        std::swap(matrix[i], matrix[j]);
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

            for (int j = i + 1; j < matrixSize; ++j) {
                number = -matrix[j][i];
                for (int k = i; k < matrixSize; ++k) {
                    matrix[j][k] += matrix[i][k] * (number / pivot);
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
            std::cout << "The determinant of the matrix is equal to: " << result << std::endl;
            std::cout << "Calculation duration of the Gauss Elimination: " << elapsedTime << std::endl;
        }
        return elapsedTime;
    }

    std::chrono::duration<double> leibnizMethod(Matrix<T>& matrix, bool characterOutput) {
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
        return elapsedTime;
    }

    T ruleOfSarrus(Matrix<T>& matrix) {
        T result = matrix[0][0] * matrix[1][1] * matrix[2][2]
                 + matrix[0][1] * matrix[1][2] * matrix[2][0]
                 + matrix[0][2] * matrix[1][0] * matrix[2][1]
                 - matrix[0][2] * matrix[1][1] * matrix[2][0]
                 - matrix[0][0] * matrix[1][2] * matrix[2][1]
                 - matrix[0][1] * matrix[1][0] * matrix[2][2];
        return result;
    }

    std::chrono::duration<double> luDecomposition(Matrix<T>& matrix, bool characterOutput) {
        auto start = getCurrentTime();
        int matrixSize = matrix.getSize();
        T result = 1;
        Matrix<T> L(matrixSize);
        Matrix<T> U(matrixSize);
        std::vector<T> diagonalElements;
        T sum;

        for (int i = 0; i < matrixSize; ++i) {
            for (int j = i; j < matrixSize; ++j) {
                if (matrix[j][i] != 0) {
                    if (j != i) {
                        std::swap(matrix[i], matrix[j]);
                    }
                    break;
                }
            }
        }
        matrix.print();

        for (int i = 0; i < matrixSize; ++i) {

            for (int j = i; j < matrixSize; ++j) {
                sum = 0;
                for (int k = 0; k < matrixSize; ++k) {
                    sum += L[i][k] * U[k][j];
                }
                U[i][j] = matrix[i][j] - sum;
            }

            if(U[i][i] == 0) {
                result = 0;
                break;
            }
            diagonalElements.push_back(U[i][i]);

            for (int j = i; j < matrixSize; ++j) {
                if (j == i) {
                    L[i][i] = 1;
                } else {
                    sum = 0;
                    for (int k = 0; k < matrixSize; ++k) {
                        sum += L[j][k] * U[k][i];
                    }
                    L[j][i] = (matrix[j][i] - sum) / U[i][i];
                }
            }
            L.print();
            U.print();
        }

        for (int i = 0; i < diagonalElements.size(); ++i) {
            result *= diagonalElements[i];
        }
        auto end = getCurrentTime();
        std::chrono::duration<double> elapsedTime = end - start;
        if (characterOutput) {
            std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
            std::cout << "Calculation duration of the LU Decomposition: " << elapsedTime << std::endl;
        }

        return elapsedTime;
    }

    std::chrono::duration<double> laplaceMethod(Matrix<T>& matrix, laplaceVariant variant, bool characterOutput) {
        auto start = getCurrentTime();
        T result = laplaceExpansion(matrix, variant);
        auto end = getCurrentTime();

        std::chrono::duration<double> elapsedTime = end - start;
        if (characterOutput) {
            std::cout << "The determinant of the matrix is equal to: " << std::setprecision(std::numeric_limits<double>::max_digits10) << result << std::endl;
            switch (variant) {
                case FULL_LAPLACE_EXPANSION: std::cout << "Calculation duration of the Full Laplace expansion: " << elapsedTime << std::endl; break;
                case LAPLACE_RULE_OF_SARRUS: std::cout << "Calculation duration of the Laplace expansion with Rule of Sarrus: " << elapsedTime << std::endl; break;
                default: std::cerr << "Invalid variant of the Laplace Expansion." << std::endl; break;
            }
        }
        return elapsedTime;
    }

    T laplaceExpansion(Matrix<T>& matrix, laplaceVariant variant) {
        int matrixSize = matrix.getSize();
        int newRow;
        int newColumn;
        T result = 0;
        T element;
        int maxNumberOfZerosInColumn = 0;
        int maxNumberOfZerosInRow = 0;
        int indexOfRow = 0;
        int indexOfColumn = 0;

        for (int i = 0; i < matrixSize; ++i) {
            int zerosInRow = 0;
            for (int j = 0; j < matrixSize; ++j) {
                if (matrix[i][j] == 0) {
                    zerosInRow++;
                }
            }
            if (zerosInRow > maxNumberOfZerosInRow) {
                maxNumberOfZerosInRow = zerosInRow;
                indexOfRow = i;
            }
        }

        for (int j = 0; j < matrixSize; ++j) {
            int zerosInColumn = 0;
            for (int i = 0; i < matrixSize; ++i) {
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
            for (int i = 0; i < matrixSize; ++i) {
                Matrix<T> newMatrix(matrixSize - 1);
                element = matrix[indexOfRow][i];

                newRow = 0;
                for (int j = 0; j < matrixSize; ++j) {
                    if (j != indexOfRow) {
                        newColumn = 0;
                        for (int k = 0; k < matrixSize; ++k) {
                            if (k != i) {
                                newMatrix[newRow][newColumn] = matrix[j][k];
                                newColumn++;
                            }
                        }
                        newRow++;
                    }
                }
                if (newMatrix.getSize() > 1) {
                    if (element != 0) {
                        if (newMatrix.getSize() == 3) {
                            switch (variant) {
                                case FULL_LAPLACE_EXPANSION: result += element * pow(-1, indexOfRow + i) * laplaceExpansion(newMatrix, variant); break;
                                case LAPLACE_RULE_OF_SARRUS: result += element * pow(-1, indexOfRow + i) * ruleOfSarrus(newMatrix); break;
                                default: std::cerr << "Invalid variant of the Laplace Expansion" << std::endl; return -1;
                            }
                        } else {
                            result += element * pow(-1, indexOfRow + i) * laplaceExpansion(newMatrix, variant);
                        }
                    }
                } else {
                    result += element * pow(-1, indexOfRow + i) * newMatrix[0][0];
                }
            }
        } else {
            for (int i = 0; i < matrixSize; ++i) {
                Matrix<T> newMatrix(matrixSize - 1);
                element = matrix[i][indexOfColumn];

                newRow = 0;
                for (int j = 0; j < matrixSize; ++j) {
                    if (j != i) {
                        newColumn = 0;
                        for (int k = 0; k < matrixSize; ++k) {
                            if (k != indexOfColumn) {
                                newMatrix[newRow][newColumn] = matrix[j][k];
                                newColumn++;
                            }
                        }
                        newRow++;
                    }
                }
                if (newMatrix.getSize() > 1) {
                    if (element != 0) {
                        if (newMatrix.getSize() == 4) {
                            switch (variant) {
                                case FULL_LAPLACE_EXPANSION: result += element * pow(-1, indexOfColumn + i) * laplaceExpansion(newMatrix, variant); break;
                                case LAPLACE_RULE_OF_SARRUS: result += element * pow(-1, indexOfColumn + i) * ruleOfSarrus(newMatrix); break;
                                default: std::cerr << "Invalid variant of the Laplace Expansion" << std::endl; return -1;
                            }
                        } else {
                            result += element * pow(-1, indexOfColumn + i) * laplaceExpansion(newMatrix, variant);
                        }
                    }
                } else {
                    result += element * pow(-1, indexOfColumn + i) * newMatrix[0][0];
                }
            }
        }
        return result;
    }
};

#endif
