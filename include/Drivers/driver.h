#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <future>
#include <utility>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"
#include "Drivers/dutDriver.h"
#include "Drivers/refDriver.h"

namespace MVM::Driver {
class Driver {
private:
    std::shared_ptr<DriverModel> dutDriver; //!< Actual Type is DutTransDriver
    std::unique_ptr<DriverModel> refDriver; //!< Actual Type is RefTransDriver
    int transPtr = 0;
    std::vector<std::shared_ptr<MVM::Transaction::Transaction>> transactions;

public:
    Driver() = delete;
    ~Driver() = default;

    /**
     * @brief Construct a new Driver object
     * 
     * @param inTransactions transaction serial tests
     * @param inDut Child of DutUnitDriver class
     * @param inDutSimulatorDrivers user defined simulator drivers for dut
     * @param inRef Child of RefUnitDriver class
     * @param inRefSimulatorDrivers user defined simulator drivers for ref
     */
    Driver(std::vector<std::shared_ptr<MVM::Transaction::Transaction>> inTransactions, std::unique_ptr<DriverModel> inDut, std::unique_ptr<DriverModel> inRef, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : transactions(std::move(inTransactions)), dutDriver(std::make_shared<DutTransDriver>(inDut
    , inSimulatorSetIndex, inSimulatorNames)), refDriver(std::make_unique<RefTransDriver>(inRef, inSimulatorSetIndex, inSimulatorNames)) {}

    void sendTransaction(bool toRef) {
        if (toRef) {
            while (!refDriver->checkTransactionFinish()) { }
            refDriver->setTransaction(transactions[transPtr]);
        }
        else {
            while (!dutDriver->checkTransactionFinish()) { }
            dutDriver->setTransaction(transactions[transPtr]);
        }
    }

    void incTransPtr() {
        transPtr++;
    }

    int driving(const std::shared_ptr<std::string> &errorMsgRaw) {
        bool dutResult, refResult;
        if (USE_THREADS) {
            for (int i = 0; i < transactions.size(); i++) {
                auto sendDutFuture = std::async(std::launch::async, &MVM::Driver::Driver::sendTransaction, this, false);
                auto sendRefFuture = std::async(std::launch::async, &MVM::Driver::Driver::sendTransaction, this, true);
                sendDutFuture.wait();
                sendRefFuture.wait();
                incTransPtr();
                auto dutFuture = std::async(std::launch::async, &MVM::Driver::DriverModel::drivingStep, dutDriver.get(), i == transactions.size() - 1);
                auto refFuture = std::async(std::launch::async, &MVM::Driver::DriverModel::drivingStep, refDriver.get(), i == transactions.size() - 1);
                dutResult = dutFuture.get();
                refResult = refFuture.get();
            }
        }
        else {
            for (int i = 0; i < transactions.size(); i++) {
                sendTransaction(false);
                sendTransaction(true);
                incTransPtr();
                dutResult = dutDriver->drivingStep(i == transactions.size() - 1);
                refResult = refDriver->drivingStep(i == transactions.size() - 1);
            }
        }
        return -1;
    }


};

    
} // namespace MVM::Driver
