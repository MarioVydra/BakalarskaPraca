#ifndef BAKALARSKAPRACA_APP_H
#define BAKALARSKAPRACA_APP_H

#include "FileReader.h"
#include "Algorithms.h"
#include "Generator.h"
#include "FileWriter.h"

class App {
public:
    void run() {
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
        while (true) {
            int input = readIntInput("Do you want to do experiments or calculate matrix from the file? "
                                     "[ '1' - experiment, '2' - calculate matrix, '3' - exit ]", 1, 3);
            if (input == 1) {
                experiment(seed);
            } else if (input == 2) {
                calculateMatrixFromFile();
            } else {
                break;
            }
        }
    };

private:
    void experiment(unsigned int seed) {
        int method = readIntInput("Choose a method for calculating the determinant [ '1' - Gauss Elimination, '2' - Leibniz Method, "
                                  "'3' - Full Laplace Expansion, '4' - Laplace Expansion + Rule of Sarrus, '5' - LU Decomposition ].", 1, 5);
        int minValue = readIntInput("Enter minimum integer for the generator.", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        int maxValue = readIntInput("Enter maximum integer for the generator.", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        double zeroProbability = readZeroProbability("Enter probability of zero elements in the matrix [0.0-1.0]");
        int dataType = readIntInput("Choose data type for generated elements [ '1' - double, '2' - float, '3' - int ]", 1, 3);

        if (dataType == 1) {
            Generator<double> generator(minValue, maxValue, zeroProbability, seed);
            replications<double, double>(method, generator);

        } else if (dataType == 2) {
            Generator<float> generator(minValue, maxValue, zeroProbability, seed);
            replications<float, float>(method, generator);
        } else {
            Generator<int> generator(minValue, maxValue, zeroProbability, seed);
            replications<double, int>(method, generator);
        }
    }

    template<typename T, typename G>
    void replications(int method, Generator<G>& generator) {
        int matrixSize = 10;
        FileWriter fileWriter("experiment" + std::to_string(method));
        Algorithms alg = Algorithms<T>();
        while (matrixSize < 101) {
            fileWriter.writeToFile(std::to_string(matrixSize) + "x" + std::to_string(matrixSize) + ": ");
            for (int i = 0; i < 100; ++i) {
                Matrix<T> matrix(matrixSize);
                matrix.generateValues(generator);
                std::chrono::duration<double> time{};
                switch (method) {
                    case 1: time = alg.gaussEliminationMethod(matrix, true); break;
                    case 2: time = alg.leibnizMethod(matrix, false); break;
                    case 3: time = alg.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION, false); break;
                    case 4: time = alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS, false); break;
                    case 5: time = alg.luDecomposition(matrix, false); break;
                    default: time = alg.gaussEliminationMethod(matrix, false); break;
                }
                fileWriter.writeToFile(std::to_string(time.count()) + " ");
            }
            fileWriter.writeToFile("\n");
            matrixSize++;
        }
    }

    void calculateMatrixFromFile() {
        std::string dataType;
        int size;
        std::string fileName = readFileNameInput("Enter the name of the file in the .txt format:");
        int method = readIntInput("Choose a method for calculating the determinant [ '1' - Gauss Elimination, '2' - Leibniz Method, "
                                  "'3' - Full Laplace Expansion, '4' - Laplace Expansion + Rule of Sarrus, '5' - LU Decomposition ].", 1, 5);
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

    static int readIntInput(const std::string& prompt, int minValue, int maxValue) {
        int input;
        std::cout << prompt << std::endl;
        while (!(std::cin >> input) || (input < minValue || input > maxValue)) {
            std::cout << "Invalid input. Please enter valid number: [" << minValue << "-" << maxValue << "]." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }

    static double readZeroProbability(const std::string& prompt) {
        double input;
        std::cout << prompt << std::endl;
        while (!(std::cin >> input) || (input < 0.0 || input > 1.0)) {
            std::cout << "Invalid input. Please enter valid number: [0.0-1.0]." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }

    static std::string readFileNameInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt << std::endl;
        while (!(std::cin >> input) || (input.length() < 4 || input.substr(input.length() - 4, input.length()) != ".txt")) {
            std::cout << "Invalid input. Please enter valid name of the file in the .txt format." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }
};
#endif
