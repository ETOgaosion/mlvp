/**
 * @file util.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "Library/utils.h"

using namespace std;
using namespace MLVP::Library;
using namespace MLVP::Type;

Data RandomGenerator::getRandomData(Data maxVal, bool throughCache, Data cacheKey) {
    std::uniform_int_distribution<Data> distribution(0, maxVal);
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

Data RandomGenerator::getRandomData(Data maxVal, bool isBitWidth) {
    if (isBitWidth) {
        std::uniform_int_distribution<Data> distribution(0, (Data)std::pow(2, maxVal) - 1);
        return distribution(rng);
    } else {
        std::uniform_int_distribution<Data> distribution(0, maxVal);
        return distribution(rng);
    }
}

Data RandomGenerator::getRandomData(Data maxVal, bool isBitWidth, bool throughCache, Data cacheKey) {
    if (isBitWidth) {
        std::uniform_int_distribution<Data> distribution(0, (Data)std::pow(2, maxVal) - 1);
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
        std::uniform_int_distribution<Data> distribution(0, maxVal);
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