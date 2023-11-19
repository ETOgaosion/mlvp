#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <future>

#include "Drivers/dutUnitDriver.h"
#include "Drivers/refUnitDriver.h"
#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"
#include "Drivers/dutDriver.h"
#include "Drivers/refDriver.h"

namespace MVM {
namespace Driver {
class Driver {
private:
    std::unique_ptr<DriverModel> dutDriver; //!< Actual Type is DutTransDriver
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
    Driver(std::vector<std::shared_ptr<MVM::Transaction::Transaction>> inTransactions, std::unique_ptr<DriverModel> inDut, std::unique_ptr<DriverModel> inRef, int inDutSimulatorSetIndex, int inRefSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : transactions(inTransactions), dutDriver(std::make_unique<DutTransDriver>(inDut
    , inDutSimulatorSetIndex, inSimulatorNames)), refDriver(std::make_unique<RefTransDriver>(inRef, inRefSimulatorSetIndex, inSimulatorNames)) {}

    void sendTransaction(bool toRef) {
        if (toRef) {
            while (!refDriver->checkTransactionFinish()) { continue; }
            refDriver->setTransaction(transactions[transPtr]);
        }
        else {
            while (!dutDriver->checkTransactionFinish()) { continue; }
            dutDriver->setTransaction(transactions[transPtr]);
        }
    }

    void incTransPtr() {
        transPtr++;
    }

    int driving(std::shared_ptr<std::string> errorMsgRaw) {
        bool dutResult, refResult;
        for (auto transaction : transactions) {
            if (USE_THREADS) {
                auto sendDutFuture = std::async(std::launch::async, &MVM::Driver::Driver::sendTransaction, this, false);
                auto sendRefFuture = std::async(std::launch::async, &MVM::Driver::Driver::sendTransaction, this, true);
                sendDutFuture.wait();
                sendRefFuture.wait();
                incTransPtr();
                auto dutFuture = std::async(std::launch::async, &MVM::Driver::DriverModel::drivingStep, dutDriver.get());
                auto refFuture = std::async(std::launch::async, &MVM::Driver::DriverModel::drivingStep, refDriver.get());
                dutResult = dutFuture.get();
                refResult = refFuture.get();
            }
            else {
                sendTransaction(false);
                sendTransaction(true);
                incTransPtr();
                dutResult = dutDriver->drivingStep();
                refResult = refDriver->drivingStep();
            }
        }
        return -1;
    }


};

    
} // namespace Driver

    
} // namespace MVM
