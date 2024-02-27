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
        outputFile_ = std::ofstream(fileName_, std::ios::app);
    };

    bool writeToFile(const std::string& data) {
        if (!outputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return false;
        }

        outputFile_ << data;
        return true;
    }
};

#endif
