#include "FileHandler.h"
#include "Algorithms.h"

int main() {
    std::string file, dataType;
    int size;

    std::cout << "Enter data type of the matrix [ float | double ]:" << std::endl;
    std::cin >> dataType;
    std::cout << "Enter the size of the matrix:" << std::endl;
    std::cin >> size;
    std::cout << "Enter the name of the file:" << std::endl;
    std::cin >> file;

    FileHandler fileHandler(file);
    Algorithms alg = Algorithms();

    if (dataType == "float") {
        Matrix<float> matrix(size);
        fileHandler.readFile(matrix);
        alg.gaussEliminationMethod(matrix);
    } else if (dataType == "double") {
        Matrix<double> matrix(size);
        fileHandler.readFile(matrix);
        alg.gaussEliminationMethod(matrix);
    } else {
        std::cerr << "Invalid data type of the matrix.";
        return 1;
    }
    return 0;
}
