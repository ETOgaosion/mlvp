/**
 * @file util.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <chrono>
#include <random>
#include <cmath>

#include "Library/types.h"

namespace MLVP::Library {
class RandomGenerator {
private:
    std::mt19937 rng;
    RandomGenerator() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

public:
    ~RandomGenerator() = default;

    static RandomGenerator &getInstance() {
        static RandomGenerator instance;
        return instance;
    }

    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal) {
        std::uniform_int_distribution<MLVP::Type::Data> distribution(0, maxVal);
        return distribution(rng);
    }

    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal, bool isBitWidth) {
        if (isBitWidth) {
            std::uniform_int_distribution<MLVP::Type::Data> distribution(0, (MLVP::Type::Data)std::pow(2, maxVal) - 1);
            return distribution(rng);
        } else {
            std::uniform_int_distribution<MLVP::Type::Data> distribution(0, maxVal);
            return distribution(rng);
        }
    }
};

} 