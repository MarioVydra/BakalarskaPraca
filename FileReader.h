#ifndef BAKALARSKAPRACA_FILEREADER_H
#define BAKALARSKAPRACA_FILEREADER_H

#include <string>
#include "Matrix.h"

class FileReader {
private:
    std::string fileName_;
    std::ifstream inputFile_;
    std::ofstream outputFile_;
public:
    FileReader(const std::string& fileName) : fileName_(fileName) {
        inputFile_ = std::ifstream(fileName_);
    };

    bool readInitializationInfo(int& size, std::string& dataType) {
        if (!inputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return false;
        }

        inputFile_ >> size >> dataType;
        return true;
    }

    template<typename T>
    bool fillMatrix(Matrix<T>& matrix) {
        if (!inputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return false;
        }

        T value;

        for (int i = 0; i < matrix.getSize(); ++i) {
            for (int j = 0; j < matrix.getSize(); ++j) {
                inputFile_ >> value;
                matrix.setValue(i, j, value);
            }
        }
        return true;
    };

};


#endif
