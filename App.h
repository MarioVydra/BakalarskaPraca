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
private:
    Algorithms algorithms_;     // algoritmy
    FileWriter fileWriter_;     // zapisovač
    FileReader fileReader_;     // čítač
public:
    /**
     * Default konštruktor triedy.
     */
    App() {
        algorithms_ = Algorithms();
        fileReader_ = FileReader();
        fileWriter_ = FileWriter();
    }

    /**
     * Metóda run spustí hlavné menu.
     */
    void run() {
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count(); // nastaví seed na základe systémového času pre vykonávanie experimentov
        while (true) {
            int input = readIntInput("Choose the action [ '1' - experiment, '2' - calculate matrix, '3' - exit ].", 1, 3);
            if (input == 1) {
                specifyExperiment(seed);
            } else if (input == 2) {
                calculateMatrixFromFile();
            } else {
                break;
            }
        }
    };

private:
    /**
     * Metóda specifyExperiment slúži na nastavenie experimentu na základe používateľského vstupu.
     *
     * @param seed seed pre generátory
     */
    void specifyExperiment(unsigned int seed) {
        int method = readIntInput("Choose the method for the experiment [ '1' - Gauss Elimination, '2' - Leibniz Method, '3' - Full Laplace Expansion, '4' - Laplace Expansion + Rule of Sarrus, '5' - LU Decomposition, '6' - Exit ].", 1, 6);
        if (method == 6) {
            return;
        }
        int minSizeOfMatrix = readIntInput("Enter the minimum size of the matrix on which you want to perform replications [3-1000].", 3, 1000);
        int maxSizeOfMatrix = readIntInput("Enter the maximum size of the matrix on which you want to perform replications [3-1000].", 3, 1000);
        int multiplesOfMatrixSize = readIntInput("Enter the multiples of matrices size on which you want to perform replications [1-100].", 1, 100);
        int numberOfReplications = readIntInput("Enter the number of the replications [1-10000].", 1, 10000);
        int seedSet = readIntInput("Do you want to set the seed? ['0' - no, '1' - yes].", 0, 1);
        if (seedSet == 1) {
            seed = readUnsignedIntInput("Enter the seed for the generator.", 0, std::numeric_limits<unsigned int>::max());
        }
        double zeroProbability = readDoubleInput("Enter the probability of zero elements in the matrix [0.0-1.0].", 0.0, 1.0);
        int dataType = readIntInput("Choose the data type for generated elements [ '1' - double, '2' - float, '3' - int ].", 1, 3);

        double minValue, maxValue;
        if (dataType == 3) {
            minValue = readIntInput("Enter the minimum integer for the generator.", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
            maxValue = readIntInput("Enter the maximum integer for the generator.", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        } else {
            minValue = readDoubleInput("Enter the minimum double for the generator.", std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
            maxValue = readDoubleInput("Enter the maximum double for the generator.", std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        }

        std::string fileName = readFileNameInput("Enter the name of the file in the .csv format to save the results:", ".csv");
        fileWriter_.setOutputFile(fileName);
        switch (method) {
            case 1: fileWriter_.writeStringToFile("Gauss Elimination\n"); break;
            case 2: fileWriter_.writeStringToFile("Leibniz method\n"); break;
            case 3: fileWriter_.writeStringToFile("Full Laplace Expansion\n"); break;
            case 4: fileWriter_.writeStringToFile("Laplace Expansion + Rule of Sarrus\n"); break;
            default: fileWriter_.writeStringToFile("LU Decomposition\n"); break;
        }
        fileWriter_.writeStringToFile("Seed;" + std::to_string(seed) + "\n");
        fileWriter_.writeStringToFile("Number of replications for each size of matrix;" + std::to_string(numberOfReplications) + "\n");
        fileWriter_.writeStringToFile("Minimum matrix size;" + std::to_string(minSizeOfMatrix) + ";Maximum matrix size;" + std::to_string(maxSizeOfMatrix) + "\n");
        fileWriter_.writeStringToFile("Zero probability;" + std::to_string(zeroProbability) + "\n");
        fileWriter_.writeStringToFile("Minimum possible generated value;" + std::to_string(minValue) + ";Maximum possible generated value;" + std::to_string(maxValue) + "\n");
        switch (dataType) {
            case 1: fileWriter_.writeStringToFile("Data type of generated values;Double\n"); break;
            case 2: fileWriter_.writeStringToFile("Data type of generated values;Float\n"); break;
            default: fileWriter_.writeStringToFile("Data type of generated values;Int\n"); break;
        }

        if (dataType == 1) {
            Generator<double> generator(minValue, maxValue, zeroProbability, seed);
            performReplications<double, double>(method, generator, numberOfReplications, minSizeOfMatrix, maxSizeOfMatrix, multiplesOfMatrixSize);

        } else if (dataType == 2) {
            Generator<float> generator(minValue, maxValue, zeroProbability, seed);
            performReplications<float, float>(method, generator, numberOfReplications, minSizeOfMatrix, maxSizeOfMatrix, multiplesOfMatrixSize);
        } else {
            Generator<int> generator(minValue, maxValue, zeroProbability, seed);
            performReplications<double, int>(method, generator, numberOfReplications, minSizeOfMatrix, maxSizeOfMatrix, multiplesOfMatrixSize);
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
     * @param minSizeOfMatrix minimálne veľkostí matíc, na ktorých budú vykopnané replikácie
     * @param maxSizeOfMatrix maximálne veľkostí matíc, na ktorých budú vykopnané replikácie
     * @param multiplesOfMatrixSize násobky veľkostí matíc, na ktorých budú vykopnané replikácie (napr. pri hodnote 10, budeme testovať stupne matíc po 10,20,30..)
     * @param fileWriter_ referencia na zapisovač
     */
    template<typename T, typename G>
    void performReplications(int method, Generator<G>& generator, int numberOfReplications, int minSizeOfMatrix, int maxSizeOfMatrix, int multiplesOfMatrixSize) {
        int matrixSize = minSizeOfMatrix;
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

        while (matrixSize < maxSizeOfMatrix + 1) {
            sumTime = 0;
            sumTimeSquared = 0;
            for (int i = 0; i < numberOfReplications; ++i) {
                Matrix<T> matrix(matrixSize);
                matrix.generateValues(generator);
                switch (method) {
                    case 1: time = algorithms_.gaussEliminationMethod(matrix, false); break;
                    case 2: time = algorithms_.leibnizMethod(matrix, false); break;
                    case 3: time = algorithms_.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION, false); break;
                    case 4: time = algorithms_.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS, false); break;
                    case 5: time = algorithms_.luDecomposition(matrix, false); break;
                    default: time = algorithms_.gaussEliminationMethod(matrix, false); break;
                }
                sumTimeSquared += std::pow(time, 2);
                sumTime += time;
            }
            avgTime = sumTime / numberOfReplications;
            standardDeviation = std::sqrt((sumTimeSquared - (std::pow(sumTime, 2) / numberOfReplications)) / (numberOfReplications - 1));
            halfWidth = (standardDeviation * 1.96) / std::sqrt(numberOfReplications);
            lowerLimit = avgTime - halfWidth;
            upperLimit = avgTime + halfWidth;

            matrixSizes.push_back(matrixSize);
            averageTimes.push_back(avgTime);
            lowerLimits.push_back(lowerLimit);
            upperLimits.push_back(upperLimit);

            matrixSize += multiplesOfMatrixSize;
        }
        fileWriter_.writeStringToFile("Matrix size;");
        for (int i = 0; i < matrixSizes.size(); ++i) {
            fileWriter_.writeStringToFile(std::to_string(matrixSizes[i]));
            fileWriter_.writeStringToFile(";");
        }
        fileWriter_.writeStringToFile("\nAverage time;");
        for (int i = 0; i < averageTimes.size(); ++i) {
            fileWriter_.writeDoubleToFile(averageTimes[i]);
            fileWriter_.writeStringToFile(";");
        }
        fileWriter_.writeStringToFile("\n95% Confidence interval (lower limit);");
        for (int i = 0; i < lowerLimits.size(); ++i) {
            fileWriter_.writeDoubleToFile(lowerLimits[i]);
            fileWriter_.writeStringToFile(";");
        }
        fileWriter_.writeStringToFile("\n95% Confidence interval (upper limit);");
        for (int i = 0; i < upperLimits.size(); ++i) {
            fileWriter_.writeDoubleToFile(upperLimits[i]);
            fileWriter_.writeStringToFile(";");
        }
        fileWriter_.writeStringToFile("\n\n");
        fileWriter_.closeFile();
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
        std::string fileName = readFileNameInput("Enter the name of the file in the .txt format to load the matrix:", ".txt");
        fileReader_.setInputFile(fileName);
        fileReader_.readInitializationInfo(size, dataType);

        std::cout << std::endl;
        if (dataType == "float") {
            calculation<float>(size, method);
        } else if (dataType == "double") {
            calculation<double>(size, method);
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
    void calculation(int size, int method) {
        Matrix<T> matrix(size);
        fileReader_.fillMatrix(matrix);

        switch (method) {
            case 1: algorithms_.gaussEliminationMethod(matrix, true); break;
            case 2: algorithms_.leibnizMethod(matrix, true); break;
            case 3: algorithms_.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION, true); break;
            case 4: algorithms_.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS, true); break;
            case 5: algorithms_.luDecomposition(matrix, true); break;
            default: algorithms_.gaussEliminationMethod(matrix, true); break;
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
    static double readDoubleInput(const std::string& prompt, double minValue, double maxValue) {
        double input;
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
     * Metóda readFileNameInput slúži na načítanie mena súboru od používateľa.
     *
     * @param prompt výzva na zadanie vstupu
     * @param fileType žiadaný typ súboru
     * @return načítaný vstup od používateľa
     */
    static std::string readFileNameInput(const std::string& prompt, const std::string& fileType) {
        std::string input;
        std::cout << prompt << std::endl;
        // pokiaľ užívateľ nezadá platný vstup
        while (!(std::cin >> input) || (input.length() < 4 || input.substr(input.length() - 4, input.length()) != fileType)) {
            std::cout << "Invalid input. Please enter valid name of the file in the " + fileType + " format." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }
};
#endif
