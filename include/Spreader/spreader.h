/**
 * @file spreader.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <future>
#include <vector>
#include <memory>
#include <iostream>

#include "Drivers/driver.h"
#include "Drivers/simulatorDriver.h"
#include "Reporter/reporter.h"
#include "Transaction/transaction.h"
#include "Database/database.h"

namespace MLVP::Spreader {
template <class TDut, class TRef, class TReport>
class Spreader
{
private:
    std::vector<std::shared_ptr<MLVP::Driver::Driver>> driver;
    std::shared_ptr<MLVP::Reporter::Reporter> reporter;
    std::vector<std::future<int>> threadsPools;
    std::vector<std::shared_ptr<std::string>> errorMsgsPool;

public:
    Spreader() = delete;
    ~Spreader() = default;

    /**
     * @brief Construct a new Spreader object
     * 
     * @param inLogPath 
     * @param inReportPath 
     * @param inSimulatorDrivers this is a complex data structure, consider example below:
     *
     * ```txt
     * {
     *   <{{"memoryDut": dutMemoryDriver1}, {"mmioDut": dutMmioDriver1}}, {{"memoryRef": refMemoryDriver1}, {"mmioRef": refMmioDriver1}}>,
     *   ....
     * }
     * ```
     *
     */
    Spreader(std::string inLogPath, std::string inReportPath, std::vector<std::pair<std::unordered_map<std::string, std::shared_ptr<MLVP::Driver::DriverModel>>, std::unordered_map<std::string, std::shared_ptr<MLVP::Driver::DriverModel>>>> inSimulatorDrivers) : reporter(std::make_shared<TReport>(inLogPath, inReportPath)) {
        driver.clear();
        threadsPools.clear();
        auto inSize = inSimulatorDrivers.size();
        for (int i = 0; i < inSize; i++) {
            driver.emplace_back(std::make_shared<MLVP::Driver::Driver>(MLVP::Database::TransactionDatabase::getInstance().getTransaction(i), std::make_shared<TDut>(i, inLogPath), std::make_shared<TRef>(), inSimulatorDrivers[i].first, inSimulatorDrivers[i].second));
            errorMsgsPool.push_back(std::make_shared<std::string>(""));
        }
    }

    void execute(){
        if (USE_THREADS) {
            for (int i = 0; i < driver.size(); i++) {
                threadsPools.push_back(std::async(std::launch::async, &MLVP::Driver::Driver::driving, driver[i].get(), errorMsgsPool[i]));
            }
            for (int i = 0; i < errorMsgsPool.size(); i++) {
                if(threadsPools[i].get() >= 0) {
                    std::cout << errorMsgsPool[i].get() << std::endl;
                }
            }
        }
        else {
            for (int i = 0; i < driver.size(); i++) {
                int result = driver[i]->driving(errorMsgsPool[i]);
            }
        }
        reporter->makeReport();
    }
};

} // namespace MLVP::Spreader