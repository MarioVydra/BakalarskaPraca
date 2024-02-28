#ifndef BAKALARSKAPRACA_GENERATOR_H
#define BAKALARSKAPRACA_GENERATOR_H

#include <iostream>
#include <random>

template<class T>
class Generator {
private:
    std::default_random_engine numberGenerator_;
    std::uniform_real_distribution<T> numberDistribution_;
    std::default_random_engine zeroGenerator_;
    std::uniform_real_distribution<double> zeroDistribution_;
    double zeroProbability_;
    unsigned int seed_;
public:
    Generator(int minValue, int maxValue, double zeroProbability, unsigned int seed) : numberDistribution_(minValue, maxValue), zeroProbability_(zeroProbability), zeroDistribution_(0.0, 1.0), seed_(seed) {
        numberGenerator_ = std::default_random_engine(seed_);
        zeroGenerator_ = std::default_random_engine(seed_);
    }

    T generate() {
        double probabilityOfZero = zeroDistribution_(zeroGenerator_);

        if (probabilityOfZero < zeroProbability_) {
            return 0.0;
        }
        double randomNumber = numberDistribution_(numberGenerator_);
        return randomNumber;
    }
};

template<>
class Generator<int> {
private:
    std::default_random_engine numberGenerator_;
    std::uniform_int_distribution<int> numberDistribution_;
    std::default_random_engine zeroGenerator_;
    std::uniform_real_distribution<double> zeroDistribution_;
    double zeroProbability_;
    unsigned int seed_;
public:
    Generator(int minValue, int maxValue, double zeroProbability, unsigned int seed) : numberDistribution_(minValue, maxValue), zeroProbability_(zeroProbability), zeroDistribution_(0.0, 1.0), seed_(seed) {
        numberGenerator_ = std::default_random_engine(seed_);
        zeroGenerator_ = std::default_random_engine(seed_);
    }

    int generate() {
        double probabilityOfZero = zeroDistribution_(zeroGenerator_);

        if (probabilityOfZero < zeroProbability_) {
            return 0.0;
        }
        int randomNumber = numberDistribution_(numberGenerator_);
        return randomNumber;
    }
};

#endif
