#ifndef BAKALARSKAPRACA_FILEWRITER_H
#define BAKALARSKAPRACA_FILEWRITER_H

#include <string>
#include <iostream>
#include <fstream>

class FileWriter {
private:
    std::string fileName_;
    std::ofstream outputFile_;
public:
    FileWriter(const std::string& fileName) : fileName_(fileName) {
        outputFile_ = std::ofstream(fileName_);
    };

    bool writeStringToFile(const std::string& data) {
        if (!outputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return false;
        }

        outputFile_ << data;
        return true;
    }

    bool writeDoubleToFile(const double& data) {
        if (!outputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return false;
        }

        outputFile_ << std::setprecision(std::numeric_limits<double>::max_digits10) << data;
        return true;
    }
};

#endif
