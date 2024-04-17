#ifndef BAKALARSKAPRACA_ALGORITHMS_H
#define BAKALARSKAPRACA_ALGORITHMS_H

#include <chrono>
#include <cmath>
#include "Matrix.h"

/**
 * Enum laplaceVariant, ktorý reprezentuje varianty Laplaceovho rozvoja.
 */
enum laplaceVariant { FULL_LAPLACE_EXPANSION, LAPLACE_RULE_OF_SARRUS };

/**
 * Trieda Algorithms obsahuje jednotlivé algoritmy na výpočet determinantu štvorcových matíc.
 *
 * @tparam T dátový typ prvkov matice
 */
template<class T>
class Algorithms {
public:
    /**
     * Getter pre aktuálny systémový čas.
     *
     * @return aktuálny systémový čas
     */
    auto getCurrentTime() {
        return std::chrono::system_clock::now();
    }

    /**
     * Metóda outputResults slúži na výpis výsledného determinantu matice a čas trvania daného výpočtu.
     *
     * @param methodName názov metódy
     * @param result výsledný determinant
     * @param elapsedTime výsledný čas trvania výpočtu
     */
    void outputResults(const std::string& methodName, T result, auto elapsedTime) {
        std::cout << "The determinant of the matrix is equal to: " << result << std::endl;
        std::cout << "Calculation duration of the " << methodName << ": " << elapsedTime << std::endl;
    }

    /**
     * Metóda gaussEliminationMethod reprezentuje algoritmus Gaussovej eliminácie na výpočet determinantu štvorcovej matice.
     *
     * @param matrix referencia na maticu
     * @param characterOutput či sa majú vypísať výsledky do konzoly (boolean)
     * @return čas trvania výpočtu
     */
    double gaussEliminationMethod(Matrix<T>& matrix, bool characterOutput) {
        auto start = getCurrentTime();
        std::vector<T> pivots;
        int matrixSize = matrix.getSize();
        T pivot;
        T result = 1;
        T number;
        T delta = std::numeric_limits<T>::min();

        for (int i = 0; i < matrixSize; ++i) {
            // nájdenie pivota
            pivot = 0;
            for (int j = i; j < matrixSize; ++j) {
                if (matrix[j][i] != 0) {
                    if (j != i) {
                        std::swap(matrix[i], matrix[j]);    // výmena riadkov
                        result *= (-1);                     // zmena znamienka determinantu
                    }
                    pivot = matrix[i][i];
                    pivots.push_back(pivot);
                    break;
                }
            }

            if (pivot == 0) {
                result = 0;
                break;
            }

            // úprava riadkov pod pivotom
            for (int j = i + 1; j < matrixSize; ++j) {
                number = -matrix[j][i];
                for (int k = i; k < matrixSize; ++k) {
                    matrix[j][k] += matrix[i][k] * (number / pivot);
                    // nastavenie malých hodnôt blízkych nule na 0
                    if (matrix[j][k] < delta && matrix[j][k] > -delta) {
                        matrix[j][k] = 0;
                    }
                }
            }
        }

        // vynásobenie pivotov
        if (result != 0) {
            for (int i = 0; i < pivots.size(); ++i) {
                result *= pivots[i];
            }
        }

        auto end = getCurrentTime();
        std::chrono::duration<double> elapsedTime = end - start;       // čas trvania výpočtu v sekundách

        if (characterOutput) {
            outputResults("Gauss Elimination", result, elapsedTime);
        }
        return elapsedTime.count();
    }

    /**
     * Metóda leibnizMethod reprezentuje algoritmus Leibnitzovej metódy na výpočet determinantu štvorcovej matice.
     *
     * @param matrix referencia na maticu
     * @param characterOutput či sa majú vypísať výsledky do konzoly (boolean)
     * @return čas trvania výpočtu
     */
    double leibnizMethod(Matrix<T>& matrix, bool characterOutput) {
        auto start = getCurrentTime();
        T result = 0;
        int numberOfSwaps;
        T product;
        T number;
        std::vector<int> columns;

        // inicializácia stĺpcových indexov
        for (int i = 0; i < matrix.getSize(); ++i) {
            columns.push_back(i);
        }

        do {
            product = 1;
            for (int i = 0; i < matrix.getSize(); ++i) {
                number = matrix[i][columns[i]];
                if (number == 0) {
                    product = 0;
                    break;
                }
                product *= number;           // vynásobenie prvkov
            }
            // zisteniu počtu výmen potrebných na zoradenie indexov do vzostupného poradia
            if (product != 0) {
                numberOfSwaps = 0;
                for (int i = 0; i < columns.size(); ++i) {
                    for (int j = i + 1; j < columns.size(); ++j) {
                        if (columns[i] > columns[j]) {
                            numberOfSwaps++;
                        }
                    }
                }
                if (numberOfSwaps % 2 != 0) {
                    product *= (-1);        // pri nepárnom počte výmen nastaví znamienko súčinu na záporné
                }
            }

            result += product;          // pripočítanie súčinu k výsledku
        } while (std::next_permutation(columns.begin(), columns.end()));    // pokračuje, dokým existujú ďalšie permutácie stĺpcových indexov

        auto end = getCurrentTime();

        std::chrono::duration<double> elapsedTime = end - start;        // čas trvania výpočtu v sekundách

        if (characterOutput) {
            outputResults("Leibniz method", result, elapsedTime);
        }
        return elapsedTime.count();
    }

    /**
     * Metóda ruleOfSarrus reprezentuje algoritmus Sarusovho pravidla na výpočet determinantu štvorcovej matice.
     *
     * @param matrix referencia na maticu
     * @return hodnota determinantu
     */
    T ruleOfSarrus(Matrix<T>& matrix) {
        T result = matrix[0][0] * matrix[1][1] * matrix[2][2]
                 + matrix[0][1] * matrix[1][2] * matrix[2][0]
                 + matrix[0][2] * matrix[1][0] * matrix[2][1]
                 - matrix[0][2] * matrix[1][1] * matrix[2][0]
                 - matrix[0][0] * matrix[1][2] * matrix[2][1]
                 - matrix[0][1] * matrix[1][0] * matrix[2][2];
        return result;
    }

    /**
     * Metóda luDecomposition reprezentuje algoritmus LU dekompozície na výpočet determinantu štvorcovej matice.
     *
     * @param matrix referencia na maticu
     * @param characterOutput či sa majú vypísať výsledky do konzoly (boolean)
     * @return čas trvania výpočtu
     */
    double luDecomposition(Matrix<T>& matrix, bool characterOutput) {
        auto start = getCurrentTime();
        int matrixSize = matrix.getSize();
        T result = 1;
        Matrix<T> L(matrixSize);
        Matrix<T> U(matrixSize);
        std::vector<T> diagonalElements;
        T sum;

        // usporiadanie riadkov matice, aby na diagonále neboli 0
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = i; j < matrixSize; ++j) {
                if (matrix[j][i] != 0) {
                    if (j != i) {
                        std::swap(matrix[i], matrix[j]);    // výmena riadkov
                        result *= (-1);                     // zmena znamienka determinantu
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < matrixSize; ++i) {
            // výpočet prvkov matice U
            for (int j = i; j < matrixSize; ++j) {
                sum = 0;
                for (int k = 0; k < matrixSize; ++k) {
                    sum += L[i][k] * U[k][j];
                }
                U[i][j] = matrix[i][j] - sum;
            }

            // kontrola, či sa nenachádza na diagonále U nula
            if(U[i][i] == 0) {
                result = 0;
                break;
            }
            diagonalElements.push_back(U[i][i]);

            // výpočet prvkov matice L
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
        }

        // vynásobenie diagonálnych prvkov matice U
        if (result != 0) {
            for (int i = 0; i < diagonalElements.size(); ++i) {
                result *= diagonalElements[i];
            }
        }
        auto end = getCurrentTime();
        std::chrono::duration<double> elapsedTime = end - start;        // čas trvania výpočtu v sekundách

        if (characterOutput) {
            outputResults("LU Decomposition", result, elapsedTime);
        }

        return elapsedTime.count();
    }

    /**
     * Metóda laplaceMethod slúži na volanie algoritmu Laplaceovho rozvoja a zistenie dĺžky trvania algoritmu Laplaceovho rozvoja.
     *
     * @param matrix referencia na maticu
     * @param variant varianta laplaceovho rozvoja
     * @param characterOutput či sa majú vypísať výsledky do konzoly (boolean)
     * @return čas trvania výpočtu
     */
    double laplaceMethod(Matrix<T>& matrix, laplaceVariant variant, bool characterOutput) {
        auto start = getCurrentTime();
        T result = laplaceExpansion(matrix, variant);
        auto end = getCurrentTime();

        std::chrono::duration<double> elapsedTime = end - start;        // čas trvania výpočtu v sekundách
        if (characterOutput) {
            switch (variant) {
                case FULL_LAPLACE_EXPANSION: outputResults("Full Laplace expansion", result, elapsedTime); break;
                case LAPLACE_RULE_OF_SARRUS: outputResults("Laplace expansion with Rule of Sarrus", result, elapsedTime); break;
                default: std::cerr << "Invalid variant of the Laplace Expansion." << std::endl; break;
            }
        }
        return elapsedTime.count();
    }

    /**
     * Metóda laplaceExpansion reprezentuje algoritmus Laplaceovho rozvoja na výpočet determinantu štvorcovej matice.
     *
     * @param matrix referencia na maticu
     * @param variant varianta laplaceovho rozvoja
     * @return hodnota determinantu
     */
    T laplaceExpansion(Matrix<T>& matrix, laplaceVariant variant) {
        int matrixSize = matrix.getSize();

        // ak je matica veľkosti 3 a varianta je Laplaceov rozvoj s použitím Sarusovho pravidla, tak vráti hodnotu determinantu vypočítanú Sarusovým pravidlom
        if (matrixSize == 3 && variant == laplaceVariant::LAPLACE_RULE_OF_SARRUS) {
            return ruleOfSarrus(matrix);
        }

        int newRow;
        int newColumn;
        T result = 0;
        T element;
        int maxNumberOfZerosInColumn = 0;
        int maxNumberOfZerosInRow = 0;
        int indexOfRow = 0;
        int indexOfColumn = 0;

        // nájdenie riadku s najväčším počtom núl
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

        // nájdenie stĺpca s najväčším počtom núl
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

        // na základe počtu núl, bude výpočet pokračovať buď podľa riadka alebo podľa stĺpca
        if (maxNumberOfZerosInRow >= maxNumberOfZerosInColumn) {
            for (int i = 0; i < matrixSize; ++i) {
                // vytvorenie nových matíc (o jeden stupeň menších)
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
                // na základe veľkostí nových matíc a hodnoty elementu pokračuje vo výpočte
                if (newMatrix.getSize() > 1) {
                    if (element != 0) {
                        result += element * std::pow(-1, indexOfRow + i) * laplaceExpansion(newMatrix, variant);
                    }
                } else {
                    result += element * std::pow(-1, indexOfRow + i) * newMatrix[0][0];
                }
            }
        } else {
            for (int i = 0; i < matrixSize; ++i) {
                // vytvorenie nových matíc (o jeden stupeň menších)
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
                // na základe veľkostí nových matíc a hodnoty elementu pokračuje vo výpočte
                if (newMatrix.getSize() > 1) {
                    if (element != 0) {
                         result += element * std::pow(-1, indexOfColumn + i) * laplaceExpansion(newMatrix, variant);
                    }
                } else {
                    result += element * std::pow(-1, indexOfColumn + i) * newMatrix[0][0];
                }
            }
        }
        return result;
    }
};

#endif
