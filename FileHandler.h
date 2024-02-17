#ifndef BAKALARSKAPRACA_FILEHANDLER_H
#define BAKALARSKAPRACA_FILEHANDLER_H

#include <string>
#include "Matrix.h"

class FileHandler {
private:
    std::string fileName_;
public:
    FileHandler(const std::string& fileName) : fileName_(fileName) {};

    template<typename T>
    bool readFile(Matrix<T>& matrix) {
        std::ifstream inputFile(fileName_);

        if (!inputFile.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return false;
        }

        T value;

        for (int i = 0; i < matrix.getSize(); ++i) {
            for (int j = 0; j < matrix.getSize(); ++j) {
                inputFile >> value;
                matrix.setValue(i, j, value);
            }
        }
        return true;
    };
};


#endif
