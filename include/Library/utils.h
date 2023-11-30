/**
 * @file util.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <chrono>
#include <random>
#include <cmath>
#include <chrono>
#include <mutex>
#include <shared_mutex>
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

    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal, bool throughCache, MLVP::Type::Data cacheKey);

    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal, bool isBitWidth);

    /**
     * @brief Get the Random Data object
     * 
     * @param maxVal maximum value to generate random data
     * @param isBitWidth whether the maxVal is bit width
     * @param throughCache whether the generation through cache, it helps dut and ref get same result
     * @param cacheKey a key to use for cache behavior, can be a request address for memory for example
     * @return MLVP::Type::Data 
     */
    MLVP::Type::Data getRandomData(MLVP::Type::Data maxVal, bool isBitWidth, bool throughCache, MLVP::Type::Data cacheKey);
};


/**
 * @brief Global timer to synchronize the clock of all components in async and multithread mode, this class use machine timer as clock, same for all components
 * @details Usage:
 * - use singleton pattern
 * - getInstance to get global timer
 * - use setUnitTimeInterval when initialize
 * - use syncTime to synchronize the clock
 * 
 */
class GlobalMachineTimer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    int unitTimeInterval = 0;
    std::mutex timerMutex;

    GlobalMachineTimer() : startTime(std::chrono::high_resolution_clock::now()) {}

public:
    ~GlobalMachineTimer() = default;

    static GlobalMachineTimer &getInstance() {
        static GlobalMachineTimer instance;
        return instance;
    }

    /**
     * @brief Set the Unit Time Interval object
     * 
     * @param unitTimeInterval
     */
    void setUnitTimeInterval(int inUnitTimeInterval) {
        std::lock_guard<std::mutex> lock(timerMutex);
        if (this->unitTimeInterval) {
            throw std::runtime_error("unit time interval has been set");
        }
        else {
            this->unitTimeInterval = inUnitTimeInterval;
        }
    }

    /**
     * @brief Set the Unit Time Interval object
     * 
     * @param unitTimeInterval 
     * @param force force cover the unit time interval, if not force, we will throw exception if unit time interval has been set
     */
    void setUnitTimeInterval(int inUnitTimeInterval, bool force) {
        std::lock_guard<std::mutex> lock(timerMutex);
        if (force) {
            this->unitTimeInterval = inUnitTimeInterval;
        }
        else {
            if (this->unitTimeInterval) {
                throw std::runtime_error("unit time interval has been set");
            }
            else {
                this->unitTimeInterval = inUnitTimeInterval;
            }
        }
    }

    /**
     * @brief synchronize timer, wait until the time is a multiple of 10ns
     * 
     */
    void syncTime() {
        auto now = std::chrono::high_resolution_clock::now();
        while (std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count() % unitTimeInterval) {}
    }
};

/**
 * @brief Global UserSet timer, user can determine when the timer increase, and make other element sync
 * @details Usage:
 * - use singleton pattern, use getInstance to get global timer
 * - use incTime to increase the timer
 * - use static sync to synchonisze
 * - [not neccessarily usable] use getTime to get the timer
 * 
 */
class GlobalUserTimer {
private:
    MLVP::Type::Data time = 0;
    std::shared_mutex timerMutex;

    GlobalUserTimer() = default;

public:
    ~GlobalUserTimer() = default;

    static GlobalUserTimer &getInstance() {
        static GlobalUserTimer instance;
        return instance;
    }

    void incTime() {
        std::unique_lock<std::shared_mutex> lock(timerMutex);
        time++;
    }

    MLVP::Type::Data getTime() {
        std::shared_lock<std::shared_mutex> lock(timerMutex);
        return time;
    }

    static void sync() {
        auto time = GlobalUserTimer::getInstance().getTime();
        while (GlobalUserTimer::getInstance().getTime() != time + 1) {}
    }
};

} 