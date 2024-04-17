#ifndef BAKALARSKAPRACA_FILEWRITER_H
#define BAKALARSKAPRACA_FILEWRITER_H

#include <string>
#include <iostream>
#include <fstream>

/**
 * Trieda FileWriter slúži na zápis údajov do súboru.
 */
class FileWriter {
private:
    std::string fileName_;          // meno výstupného súboru
    std::ofstream outputFile_;      // atribút zodpovedný za prácu s výstupným súborom
public:
    /**
     * Konštruktor triedy.
     *
     * @param fileName meno výstupného súboru
     */
    FileWriter(const std::string& fileName) : fileName_(fileName) {
        outputFile_ = std::ofstream(fileName_, std::ios_base::app);
    };

    /**
     * Metóda writeStringToFile slúži na zápis reťazcov do súboru.
     *
     * @param data dáta vo forme reťazca
     */
    void writeStringToFile(const std::string& data) {
        // kontrola, či je súbor otvorený
        if (!outputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return;
        }

        outputFile_ << data;
    }

    /**
     * Metóda writedoubleToFile slúži na zápis čísel do súboru.
     *
     * @param data dáta vo forme čísel
     */
    void writeDoubleToFile(const double& data) {
        // kontrola, či je súbor otvorený
        if (!outputFile_.is_open()) {
            std::cerr << "Unable to open the file: " << fileName_ << std::endl;
            return;
        }

        outputFile_ << std::setprecision(std::numeric_limits<double>::max_digits10) << data;
    }
};

#endif
