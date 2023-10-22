#pragma once

#include <future>
#include <vector>
#include <memory>
#include <iostream>

#include "Driver/driver.h"
#include "Reporter/reporter.h"
#include "Transaction/transaction.h"
#include "Database/database.h"

namespace MVM {
namespace Spreader {
template <class TDut, class TRef, class TReport>
class Spreader
{
private:
    std::vector<std::unique_ptr<MVM::Driver::Driver<TDut, TRef>>> driver;
    std::unique_ptr<MVM::Reporter::Reporter> reporter;
    std::vector<std::future<int>> threadsPools;
    std::vector<std::shared_ptr<std::string>> errorMsgsPool;

public:
    Spreader() = delete;
    ~Spreader() = default;
    Spreader(std::string inLogPath, std::string inReportPath) : reporter(std::make_unique<TReport>(inLogPath, inReportPath)) {
        driver.clear();
        threadsPools.clear();
        for (int i = 0; i < MVM::Database::TransactionDatabase::getInstance().getTransactionSize(); i++) {
            driver.push_back(std::make_unique<Driver::Driver<TDut, TRef>>(i, inLogPath, MVM::Database::TransactionDatabase::getInstance().getTransaction(i)));
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