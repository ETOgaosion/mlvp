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
#include <unordered_map>

#include "Library/types.h"

namespace MLVP::Library {
class RandomGenerator {
private:
    std::mt19937 rng;
    std::unordered_map<MLVP::Type::Data, MLVP::Type::Data> dataCache;
    RandomGenerator() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
        dataCache.clear();
    }

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

    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal, bool throughCache, MLVP::Type::Data cacheKey) {
        std::uniform_int_distribution<MLVP::Type::Data> distribution(0, maxVal);
        if (throughCache) {
            if (dataCache.contains(cacheKey)) {
                return dataCache[cacheKey];
            } else {
                dataCache[cacheKey] = distribution(rng);
                return dataCache[cacheKey];
            }
        } else {
            return distribution(rng);
        }
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

    /**
     * @brief Get the Random Data object
     * 
     * @param maxVal maximum value to generate random data
     * @param isBitWidth whether the maxVal is bit width
     * @param throughCache whether the generation through cache, it helps dut and ref get same result
     * @param cacheKey a key to use for cache behavior, can be a request address for memory for example
     * @return MLVP::Type::Data 
     */
    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal, bool isBitWidth, bool throughCache, MLVP::Type::Data cacheKey) {
        if (isBitWidth) {
            std::uniform_int_distribution<MLVP::Type::Data> distribution(0, (MLVP::Type::Data)std::pow(2, maxVal) - 1);
            if (throughCache) {
                if (dataCache.contains(cacheKey)) {
                    return dataCache[cacheKey];
                } else {
                    dataCache[cacheKey] = distribution(rng);
                    return dataCache[cacheKey];
                }
            } else {
                return distribution(rng);
            }
        } else {
            std::uniform_int_distribution<MLVP::Type::Data> distribution(0, maxVal);
            if (throughCache) {
                if (dataCache.contains(cacheKey)) {
                    return dataCache[cacheKey];
                } else {
                    dataCache[cacheKey] = distribution(rng);
                    return dataCache[cacheKey];
                }
            } else {
                return distribution(rng);
            }
        }
    }
};

} 