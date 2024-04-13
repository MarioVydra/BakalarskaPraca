#include "App.h"

/**
 * Metóda main predstavuje vstupný bod programu.
 */
int main() {
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10); // nastaví presnosť desatinných miest pri výpise na maximálny počet číslic
    App app;
    app.run();
}
