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

namespace MVM::Spreader {
template <class TDut, class TRef, class TReport>
class Spreader
{
private:
    std::vector<std::unique_ptr<MVM::Driver::Driver>> driver;
    std::unique_ptr<MVM::Reporter::Reporter> reporter;
    std::vector<std::future<int>> threadsPools;
    std::vector<std::shared_ptr<std::string>> errorMsgsPool;

public:
    Spreader() = delete;
    ~Spreader() = default;

    Spreader(std::string inUnitName, std::string inLogPath, std::string inReportPath, int inSimulatorMaxIndex, std::vector<std::string> inSimulatorNames) : reporter(std::make_unique<TReport>(inLogPath, inReportPath)) {
        if (inSimulatorMaxIndex != MVM::Database::TransactionDatabase::getInstance().getTransactionSize()) {
            throw std::runtime_error("SimulatorMaxIndex not match with TransactionDatabase");
        }
        if (inSimulatorMaxIndex != inSimulatorNames.size()) {
            throw std::runtime_error("SimulatorMaxIndex not match with SimulatorNames");
        }
        driver.clear();
        threadsPools.clear();
        for (int i = 0; i < inSimulatorMaxIndex; i++) {
            driver.emplace_back(MVM::Database::TransactionDatabase::getInstance().getTransaction(i), std::make_unique<TDut>(inUnitName, i, inLogPath), std::make_unique<TRef>(inUnitName, i, inLogPath), inSimulatorMaxIndex, inSimulatorNames);
            inSimulatorNames.clear();
            errorMsgsPool.push_back(std::make_shared<std::string>(""));
        }
    }

    void execute(){
        if (USE_THREADS) {
            for (int i = 0; i < driver.size(); i++) {
                threadsPools.push_back(std::async(std::launch::async, &MVM::Driver::Driver::driving, driver[i].get(), errorMsgsPool[i]));
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

} // namespace MVM::Spreader