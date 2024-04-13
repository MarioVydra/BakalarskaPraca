#ifndef BAKALARSKAPRACA_MATRIX_H
#define BAKALARSKAPRACA_MATRIX_H

#include <fstream>
#include <iostream>
#include <vector>
#include "Generator.h"

/**
 * Trieda Matrix reprezentuje štvorcovú maticu.
 *
 * @tparam T dátový typ prvkov matice
 */

template<class T>
class Matrix {
private:
    int size_;      // stupeň (veľkosť) štvorcovej matice
    T** matrix_;    // dvojité pole obsahujúce prvky matice
public:
    /**
     * Konštruktor pre maticu.
     * Inicializuje dvojité pole a nastaví všetky prvky na 0.
     *
     * @param size stupeň (veľkosť) štvorcovej matice
     */
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

    /**
     * Deštruktor pre maticu.
     * Uvoľní dynamicky alokovanú pamäť.
     */
    ~Matrix() {
        for (int i = 0; i < size_; ++i) {
            delete[] matrix_[i];
        }
        delete[] matrix_;
    }

    /**
     * Metóda generateValues slúži vygenerovanie hodnôt prvkov matice.
     *
     * @tparam G dátový typ generovaných dát
     * @param generator referencia na generátor, ktorý generuje hodnoty pre prvky matice
     */
    template<typename G>
    void generateValues(Generator<G>& generator) {
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                matrix_[i][j] = generator.generate();
            }
        }
    }

    /**
     * Metóda print slúži na výpis prvkov matice do konzoly.
     */
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

    /**
     * Getter pre stupeň (veľkosť) štvorcovej matice.
     *
     * @return stupeň (veľkosť) štvorcovej matice
     */
    int getSize() {
        return size_;
    };

    /**
     * Operátor [] slúži na prístup k jednotlivým prvkom matice.
     *
     * @param index index riadku matice
     * @return referenciu na pointer (riadok matice)
     */
    T*& operator[](int index) {
        return matrix_[index];
    }
};

#endif
