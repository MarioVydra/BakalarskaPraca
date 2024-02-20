#include "FileHandler.h"
#include "Algorithms.h"

int main() {
    std::string fileName, dataType;
    int size, method, variant;

    std::cout << "Enter the name of the file in the .txt format:" << std::endl;
    while (!(std::cin >> fileName) || (fileName.length() < 4 || fileName.substr(fileName.length() - 4, fileName.length()) != ".txt")) {
        std::cout << "Invalid input. Please enter valid name of the file in the .txt format." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    FileHandler fileHandler(fileName);
    Algorithms alg = Algorithms();

    fileHandler.readInitializationInfo(size, dataType);

    std::cout << "Choose a method for calculating the determinant [ '1' - Gauss Elimination, '2' - Leibniz Method, '3' - Laplace Expansion ]" << std::endl;
    while (!(std::cin >> method) || (method != 1 && method != 2 && method != 3)) {
        std::cout << "Invalid input. Please enter valid number of the calculating method." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (dataType == "float") {
        Matrix<float> matrix(size);
        fileHandler.fillMatrix(matrix);

        if (method == 1) {
            alg.gaussEliminationMethod(matrix, true);
        } else if (method == 2) {
            alg.leibnizMethod(matrix, true);
        } else {
            std::cout << "Choose a variant of Laplace Expansion [ '1' - Full Laplace Expansion, '2' - Laplace Expansion + Rule of Sarrus, '3' - Laplace Expansion + Gauss Elimination ]" << std::endl;
            while (!(std::cin >> variant) || (method != 1 && method != 2 && method != 3)) {
                std::cout << "Invalid input. Please enter valid number of the variant of Laplace Expansion." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cout << std::endl;
            if (variant == 1) {
                alg.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION);
            } else if (variant == 2) {
                alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS);
            } else {
                alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_GAUSS_ELIMINATION);
            }
        }
    } else if (dataType == "double") {
        Matrix<double> matrix(size);
        fileHandler.fillMatrix(matrix);

        if (method == 1) {
            alg.gaussEliminationMethod(matrix, true);
        } else if (method == 2) {
            alg.leibnizMethod(matrix, true);
        } else {
            std::cout << "Choose a variant of Laplace Expansion [ '1' - Full Laplace Expansion, '2' - Laplace Expansion + Rule of Sarrus, '3' - Laplace Expansion + Gauss Elimination ]" << std::endl;
            while (!(std::cin >> variant) || (method != 1 && method != 2 && method != 3)) {
                std::cout << "Invalid input. Please enter valid number of the variant of Laplace Expansion." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cout << std::endl;
            if (variant == 1) {
                alg.laplaceMethod(matrix, laplaceVariant::FULL_LAPLACE_EXPANSION);
            } else if (variant == 2) {
                alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_RULE_OF_SARRUS);
            } else {
                alg.laplaceMethod(matrix, laplaceVariant::LAPLACE_GAUSS_ELIMINATION);
            }
        }
    } else {
        std::cerr << "Invalid data type of the matrix.";
        return 1;
    }
    return 0;
}
