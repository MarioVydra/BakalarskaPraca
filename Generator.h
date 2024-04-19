#ifndef BAKALARSKAPRACA_GENERATOR_H
#define BAKALARSKAPRACA_GENERATOR_H

#include <iostream>
#include <random>

/**
 * Trieda Generator slúži na generovanie hodnôt pre prvky matice.
 *
 * @tparam T dátový typ generovaných hodnôt
 */
template<class T>
class Generator {
private:
    std::default_random_engine numberGenerator_;                    // generátor náhodných čísel
    std::uniform_real_distribution<T> numberDistribution_;          // distribúcia rovnomerne náhodných čísel
    std::default_random_engine zeroGenerator_;                      // generátor pravdepodobnosti vygenerovania 0
    std::uniform_real_distribution<double> zeroDistribution_;       // distribúcia rovnomerne náhodných čísel pre pravdepodobnosť vygenerovania 0
    double zeroProbability_;                                        // pravdepodobnosť vygenerovania 0
    unsigned int seed_;                                             // seed pre generátory
public:
    /**
     * Konštruktor pre generátor.
     *
     * @param minValue minimálna hodnota, ktorá môže byť vygenerovaná
     * @param maxValue maximálna hodnota, ktorá môže byť vygenerovaná
     * @param zeroProbability pravdepodobnosť vygenerovania 0
     * @param seed seed
     */
    Generator(double minValue, double maxValue, double zeroProbability, unsigned int seed) : numberDistribution_(minValue, maxValue), zeroProbability_(zeroProbability), zeroDistribution_(0.0, 1.0), seed_(seed) {
        numberGenerator_ = std::default_random_engine(seed_);
        zeroGenerator_ = std::default_random_engine(seed_);
    }

    /**
     * Metóda generate slúži na generovanie jednotlivých hodnôt.
     *
     * @return vygenerovaná hodnota
     */
    T generate() {
        double probabilityOfZero = zeroDistribution_(zeroGenerator_); // najskôr vygeneruje pravdepodobnosť vygenerovania nuly

        // ak je menšia ako nami zadaná pravdepodobnosť, tak vráti 0
        if (probabilityOfZero < zeroProbability_) {
            return 0.0;
        }
        double randomNumber = numberDistribution_(numberGenerator_); // vygeneruje hodnotu

        // pokiaľ vygenerovaná hodnota je rovná 0, tak bude generovať ďalšie hodnoty
        while (randomNumber == 0) {
            randomNumber = numberDistribution_(numberGenerator_);
        }
        return randomNumber;
    }
};

/**
 * Trieda Generator pomocou špecializácie šablón, slúži na generovanie celočíselných hodnôt.
 */
template<>
class Generator<int> {
private:
    std::default_random_engine numberGenerator_;                    // generátor náhodných čísel
    std::uniform_int_distribution<int> numberDistribution_;         // distribúcia rovnomerne náhodných celočíselných hodnôt
    std::default_random_engine zeroGenerator_;                      // generátor pravdepodobnosti vygenerovania 0
    std::uniform_real_distribution<double> zeroDistribution_;       // distribúcia rovnomerne náhodných čísel pre pravdepodobnosť vygenerovania 0
    double zeroProbability_;                                        // pravdepodobnosť vygenerovania 0
    unsigned int seed_;                                             // seed pre generátory
public:
    /**
     * Konštruktor pre generátor.
     *
     * @param minValue minimálna hodnota, ktorá môže byť vygenerovaná
     * @param maxValue maximálna hodnota, ktorá môže byť vygenerovaná
     * @param zeroProbability pravdepodobnosť vygenerovania 0
     * @param seed seed
     */
    Generator(int minValue, int maxValue, double zeroProbability, unsigned int seed) : numberDistribution_(minValue, maxValue), zeroProbability_(zeroProbability), zeroDistribution_(0.0, 1.0), seed_(seed) {
        numberGenerator_ = std::default_random_engine(seed_);
        zeroGenerator_ = std::default_random_engine(seed_);
    }

    /**
     * Metóda generate slúži na generovanie jednotlivých hodnôt.
     *
     * @return vygenerovaná hodnota
    */
    int generate() {
        double probabilityOfZero = zeroDistribution_(zeroGenerator_); // najskôr vygeneruje pravdepodobnosť vygenerovania nuly

        // ak je pravdepodobnosť vygenerovania nuly menšia ako nami zadaná pravdepodobnosť, tak vráti 0
        if (probabilityOfZero < zeroProbability_) {
            return 0.0;
        }
        int randomNumber = numberDistribution_(numberGenerator_); // vygeneruje hodnotu

        // pokiaľ vygenerovaná hodnota je rovná 0, tak bude generovať ďalšie hodnoty
        while (randomNumber == 0) {
            randomNumber = numberDistribution_(numberGenerator_);
        }
        return randomNumber;
    }
};

#endif
