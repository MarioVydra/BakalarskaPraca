#ifndef BAKALARSKAPRACA_FILEREADER_H
#define BAKALARSKAPRACA_FILEREADER_H

#include <string>
#include "Matrix.h"

/**
 * Trieda FileReader slúži na čítanie údajov zo súboru.
 */
class FileReader {
private:
    std::string fileName_;          // meno vstupného súboru
    std::ifstream inputFile_;       // atribút zodpovedný za manipuláciu so vstupným súborom
public:
    /**
     * Konštruktor triedy.
     *
     * @param fileName meno vstupného súboru
     */
    FileReader(const std::string& fileName) : fileName_(fileName) {
        inputFile_ = std::ifstream(fileName_);
    };

    /**
     * Metóda readInitializationInfo slúži na načítanie základných údajov o matici (dátový typ prvkov, stupeň / veľkosť matice)
     *
     * @param size veľkosť (stupeň) matice
     * @param dataType dátový typ prvkov matice
     */
    void readInitializationInfo(int& size, std::string& dataType) {
        // kontrola, či je súbor otvorený
        if (!inputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return;
        }

        inputFile_ >> size >> dataType;
    }

    /**
     * Metóda fillMatrix slúži na načítanie jednotlivých prvkov matice.
     *
     * @tparam T dátový typ prvkov matice
     * @param matrix referencia na maticu
     */
    template<typename T>
    void fillMatrix(Matrix<T>& matrix) {
        // kontrola, či je súbor otvorený
        if (!inputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return;
        }

        T value;

        for (int i = 0; i < matrix.getSize(); ++i) {
            for (int j = 0; j < matrix.getSize(); ++j) {
                inputFile_ >> value;
                matrix[i][j] = value;
            }
        }
    };

};


#endif
