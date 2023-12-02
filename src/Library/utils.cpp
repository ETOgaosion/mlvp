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
#include <mutex>

using namespace std;
using namespace MLVP::Library;
using namespace MLVP::Type;

Data RandomGenerator::getRandomData(Data maxVal, bool throughCache, Data cacheKey) {
    if (USE_THREADS && throughCache) {
        randomGeneratorMutex.lock();
    }
    uniform_int_distribution<Data> distribution(0, maxVal);
    if (throughCache) {
        if (dataCache.contains(cacheKey)) {
            auto ret = dataCache[cacheKey];
            if (USE_THREADS) {
                randomGeneratorMutex.unlock();
            }
            return ret;
        } else {
            dataCache[cacheKey] = distribution(rng);
            auto ret = dataCache[cacheKey];
            if (USE_THREADS) {
                randomGeneratorMutex.unlock();
            }
            return ret;
        }
    } else {
        return distribution(rng);
    }
}

Data RandomGenerator::getRandomData(Data maxVal, bool isBitWidth) {
    if (isBitWidth) {
        uniform_int_distribution<Data> distribution(0, (Data)pow(2, maxVal) - 1);
        return distribution(rng);
    } else {
        uniform_int_distribution<Data> distribution(0, maxVal);
        return distribution(rng);
    }
}

Data RandomGenerator::getRandomData(Data maxVal, bool isBitWidth, bool throughCache, Data cacheKey) {
    if (USE_THREADS && throughCache) {
        randomGeneratorMutex.lock();
    }
    if (isBitWidth) {
        uniform_int_distribution<Data> distribution(0, (Data)pow(2, maxVal) - 1);
        if (throughCache) {
            if (dataCache.contains(cacheKey)) {
                auto ret = dataCache[cacheKey];
                if (USE_THREADS) {
                    randomGeneratorMutex.unlock();
                }
                return ret;
            } else {
                dataCache[cacheKey] = distribution(rng);
                auto ret = dataCache[cacheKey];
                if (USE_THREADS) {
                    randomGeneratorMutex.unlock();
                }
                return ret;
            }
        } else {
            return distribution(rng);
        }
    } else {
        uniform_int_distribution<Data> distribution(0, maxVal);
        if (throughCache) {
            if (dataCache.contains(cacheKey)) {
                auto ret = dataCache[cacheKey];
                if (USE_THREADS) {
                    randomGeneratorMutex.unlock();
                }
                return ret;
            } else {
                dataCache[cacheKey] = distribution(rng);
                auto ret = dataCache[cacheKey];
                if (USE_THREADS) {
                    randomGeneratorMutex.unlock();
                }
                return ret;
            }
        } else {
            return distribution(rng);
        }
    }
}