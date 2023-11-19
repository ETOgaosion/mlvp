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

namespace MVM {
namespace Spreader {
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

    Spreader(std::string inUnitName, std::string inLogPath, std::string inReportPath, std::vector<std::vector<std::shared_ptr<SimulatorDriver>>> inDriverModels) : reporter(std::make_unique<TReport>(inLogPath, inReportPath)) {
        driver.clear();
        threadsPools.clear();
        int dutSimulatorIndex = 0, refSimulatorIndex = 0;
        std::vector<std::string> simulatorNames;
        for (int i = 0; i < MVM::Database::TransactionDatabase::getInstance().getTransactionSize(); i++) {
            dutSimulatorIndex = MVM::Driver::SimulatorlDriverRegistrar::getInstance().registerSimulatorDriver(inDriverModels[2 * i]);
            refSimulatorIndex = MVM::Driver::SimulatorlDriverRegistrar::getInstance().registerSimulatorDriver(inDriverModels[2 * i + 1]);
            for (auto& driver : inDriverModels[2 * i]) {
                simulatorNames.push_back(driver->getName());
            }
            driver.emplace_back(MVM::Database::TransactionDatabase::getInstance().getTransaction(i), std::make_unique<TDut>(inUnitName, i, inLogPath), std::make_unique<TRef>(inUnitName, i, inLogPath), dutSimulatorIndex, refSimulatorIndex, simulatorNames);
            simulatorNames.clear();
            errorMsgsPool.push_back(std::make_shared<std::string>(""));
        }
    }

    void execute(){
        if (USE_THREADS) {
            for (int i = 0; i < driver.size(); i++) {
                threadsPools.push_back(std::async(std::launch::async, &MVM::Driver::Driver<TDut, TRef>::driving, driver[i].get(), errorMsgsPool[i]));
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



} // namespace Spreader


} // namespace MVM