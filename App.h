#ifndef BAKALARSKAPRACA_APP_H
#define BAKALARSKAPRACA_APP_H

#include "FileReader.h"
#include "Algorithms.h"
#include "Generator.h"
#include "FileWriter.h"

/**
 * Trieda App je zodpovedná za beh celej aplikácie.
 */
class App {
public:
    /**
     * Metóda run spustí hlavné menu.
     */
    void run() {
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count(); // nastaví seed na základe systémového času pre vykonávanie experimentov
        while (true) {
            int input = readIntInput("Choose the action [ '1' - experiment, '2' - calculate matrix, '3' - exit ].", 1, 3);
            if (input == 1) {
                performExperiment(seed);
            } else if (input == 2) {
                calculateMatrixFromFile();
            } else {
                break;
            }
        }
    };

private:
    /**
     * Metóda performExperiment slúži na nastavenie experimentu na základe používateľského vstupu.
     *
     * @param seed seed pre generátory
     */
    void performExperiment(unsigned int seed) {
        int method = readIntInput("Choose the method for the experiment [ '1' - Gauss Elimination, '2' - Leibniz Method, '3' - Full Laplace Expansion, '4' - Laplace Expansion + Rule of Sarrus, '5' - LU Decomposition, '6' - Exit ].", 1, 6);
        if (method == 6) {
            return;
        }
        int numberOfReplications = readIntInput("Enter the number of the replications [1-10000].", 1, 10000);
        int seedSet = readIntInput("Do you want to set the seed? ['0' - no, '1' - yes].", 0, 1);
        if (seedSet == 1) {
            seed = readUnsignedIntInput("Enter the seed for the generator.", 0, std::numeric_limits<unsigned int>::max());
        }
        int minValue = readIntInput("Enter the minimum integer for the generator.", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        int maxValue = readIntInput("Enter the maximum integer for the generator.", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        double zeroProbability = readZeroProbability("Enter the probability of zero elements in the matrix [0.0-1.0].");
        int dataType = readIntInput("Choose the data type for generated elements [ '1' - double, '2' - float, '3' - int ].", 1, 3);

        if (dataType == 1) {
            Generator<double> generator(minValue, maxValue, zeroProbability, seed);
            performReplications<double, double>(method, generator, numberOfReplications);

        } else if (dataType == 2) {
            Generator<float> generator(minValue, maxValue, zeroProbability, seed);
            performReplications<float, float>(method, generator, numberOfReplications);
        } else {
            Generator<int> generator(minValue, maxValue, zeroProbability, seed);
            performReplications<double, int>(method, generator, numberOfReplications);
        }
    }

    /**
     * Metóda performReplications slúži na vykonávanie samotných replikácii.
     *
     * @tparam T dátový typ prvkov matice
     * @tparam G dátový typ generovnaých hodnôt
     * @param method vybraná metóda, s ktorou chceme experimentovať
     * @param generator generátor
     * @param numberOfReplications počet replikácii
     */
    template<typename T, typename G>
    void performReplications(int method, Generator<G>& generator, int numberOfReplications) {
        int matrixSize = 10;
        double time;
        double sumTime;
        double sumTimeSquared;
        double avgTime;
        double standardDeviation;
        double halfWidth;
        double lowerLimit;
        double upperLimit;
        std::vector<double> matrixSizes;
        std::vector<double> averageTimes;
        std::vector<double> lowerLimits;
        std::vector<double> upperLimits;

        std::string fileName = readFileNameInput("Enter the name of the file in the .txt format to save the results:");
        FileWriter fileWriter(fileName);
        Algorithms alg = Algorithms<T>();
        fileWriter.writeStringToFile("Seed: " + std::to_string(generator.getSeed()) + "\n\n");

        while (matrixSize < 101) {
            fileWriter.writeStringToFile(std::to_string(matrixSize) + "x" + std::to_string(matrixSize) + ": ");
            sumTime = 0;
            sumTimeSquared = 0;

            std::cout << matrixSize << "x" << matrixSize << ": ";
            for (int i = 0; i < numberOfReplications; ++i) {
                Matrix<T> matrix(matrixSize);
                matrix.generateValues(generator);
                //std::cout << matrix.countZeros() << " ";
                switch (method) {
                    case 1: time = alg.gaussEliminationMethod(matrix, false); break;
                    case 2: time = alg.leibnizMethod(matrix, false); break;
                    case 3: time = alg.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION, false); break;
                    case 4: time = alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS, false); break;
                    case 5: time = alg.luDecomposition(matrix, false); break;
                    default: time = alg.gaussEliminationMethod(matrix, false); break;
                }
                sumTimeSquared += std::pow(time, 2);
                sumTime += time;
                fileWriter.writeDoubleToFile(time);
                fileWriter.writeStringToFile(" ");
            }
            avgTime = sumTime / numberOfReplications;
            standardDeviation = std::sqrt((sumTimeSquared - (std::pow(sumTime, 2) / numberOfReplications)) / (numberOfReplications - 1));
            halfWidth = (standardDeviation * 1.96) / std::sqrt(100);
            lowerLimit = avgTime - halfWidth;
            upperLimit = avgTime + halfWidth;

            fileWriter.writeStringToFile("\nAverage time of " + std::to_string(matrixSize) + "x" + std::to_string(matrixSize) + " matrix: ");
            fileWriter.writeDoubleToFile(avgTime);
            fileWriter.writeStringToFile("\n95% Confidence interval of " + std::to_string(matrixSize) + "x" + std::to_string(matrixSize) + " matrix: <");
            fileWriter.writeDoubleToFile(lowerLimit);
            fileWriter.writeStringToFile(",");
            fileWriter.writeDoubleToFile(upperLimit);
            fileWriter.writeStringToFile(">\n\n");

            matrixSizes.push_back(matrixSize);
            averageTimes.push_back(avgTime);
            lowerLimits.push_back(lowerLimit);
            upperLimits.push_back(upperLimit);
            matrixSize++;
            std::cout << std::endl;
        }
    }

    /**
     * Metóda calculateMatrixFromFile slúži na načítanie matice zo súboru a na výber výpočtovej metódy.
     */
    void calculateMatrixFromFile() {
        std::string dataType;
        int size;
        int method = readIntInput("Choose the method for calculating the determinant [ '1' - Gauss Elimination, '2' - Leibniz Method, '3' - Full Laplace Expansion, '4' - Laplace Expansion + Rule of Sarrus, '5' - LU Decomposition, '6' - Exit ].", 1, 6);
        if (method == 6) {
            return;
        }
        std::string fileName = readFileNameInput("Enter the name of the file in the .txt format to load the matrix:");
        FileReader fileReader(fileName);
        fileReader.readInitializationInfo(size, dataType);

        std::cout << std::endl;
        if (dataType == "float") {
            calculation<float>(fileReader, size, method);
        } else if (dataType == "double") {
            calculation<double>(fileReader, size, method);
        } else {
            std::cerr << "Invalid data type of the matrix.";
        }
        std::cout << std::endl;
    }

    /**
     * Metóda calculation slúži na výpočet determinantu štvorcovej matice načítanej zo vstupného súboru.
     *
     * @tparam T dátový typ prvkov matice
     * @param fileReader čítač údajov zo súboru
     * @param size stupeň (veľkosť) matice
     * @param method výpočtová metóda
     */
    template<typename T>
    void calculation(FileReader& fileReader, int size, int method) {
        Matrix<T> matrix(size);
        Algorithms alg = Algorithms<T>();
        fileReader.fillMatrix(matrix);

        switch (method) {
            case 1: alg.gaussEliminationMethod(matrix, true); break;
            case 2: alg.leibnizMethod(matrix, true); break;
            case 3: alg.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION, true); break;
            case 4: alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS, true); break;
            case 5: alg.luDecomposition(matrix, true); break;
            default: alg.gaussEliminationMethod(matrix, true); break;
        }
    }

    /**
     * Metóda readIntInput slúži na načítanie celočísleného vstupu od používateľa.
     *
     * @param prompt výzva na zadanie vstupu
     * @param minValue minimálny možný input
     * @param maxValue maximálny možný input
     * @return načítaný vstup od používateľa
     */
    static int readIntInput(const std::string& prompt, int minValue, int maxValue) {
        int input;
        std::cout << prompt << std::endl;
        // pokiaľ užívateľ nezadá platný vstup
        while (!(std::cin >> input) || (input < minValue || input > maxValue)) {
            std::cout << "Invalid input. Please enter valid number: [" << minValue << "-" << maxValue << "]." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }

    /**
     * Metóda readUnsignedIntInput slúži na načítanie neznamienkového celočísleného vstupu od používateľa.
     *
     * @param prompt výzva na zadanie vstupu
     * @param minValue minimálny možný input
     * @param maxValue maximálny možný input
     * @return načítaný vstup od používateľa
     */
    static unsigned int readUnsignedIntInput(const std::string& prompt, unsigned int minValue, unsigned int maxValue) {
        unsigned int input;
        std::cout << prompt << std::endl;
        // pokiaľ užívateľ nezadá platný vstup
        while (!(std::cin >> input) || (input < minValue || input > maxValue)) {
            std::cout << "Invalid input. Please enter valid number: [" << minValue << "-" << maxValue << "]." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }

    /**
     * Metóda readZeroProbability slúži na načítanie pravdepodobnosti vygenerovania nuly od používateľa.
     *
     * @param prompt výzva na zadanie vstupu
     * @return načítaný vstup od používateľa
     */
    static double readZeroProbability(const std::string& prompt) {
        double input;
        std::cout << prompt << std::endl;
        // pokiaľ užívateľ nezadá platný vstup
        while (!(std::cin >> input) || (input < 0.0 || input > 1.0)) {
            std::cout << "Invalid input. Please enter valid number: [0.0-1.0]." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }

    /**
     * Metóda readFileNameInput slúži na načítanie mena súboru od používateľa.
     *
     * @param prompt výzva na zadanie vstupu
     * @return načítaný vstup od používateľa
     */
    static std::string readFileNameInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt << std::endl;
        // pokiaľ užívateľ nezadá platný vstup
        while (!(std::cin >> input) || (input.length() < 4 || input.substr(input.length() - 4, input.length()) != ".txt")) {
            std::cout << "Invalid input. Please enter valid name of the file in the .txt format." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }
};
#endif
