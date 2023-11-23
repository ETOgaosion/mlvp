/**
 * @file driver.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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

namespace MLVP::Driver {
class Driver {
private:
    std::shared_ptr<DutTransDriver> dutDriver; //!< Actual Type is DutTransDriver
    std::shared_ptr<RefTransDriver> refDriver; //!< Actual Type is RefTransDriver
    int transPtr = 0;
    std::vector<std::shared_ptr<MLVP::Transaction::Transaction>> transactions;

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
    Driver(std::vector<std::shared_ptr<MLVP::Transaction::Transaction>> inTransactions, std::unique_ptr<DriverModel> inDut, std::unique_ptr<DriverModel> inRef, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : transactions(std::move(inTransactions)), dutDriver(std::make_shared<DutTransDriver>(inDut
    , inSimulatorSetIndex, inSimulatorNames)), refDriver(std::make_shared<RefTransDriver>(inRef, inSimulatorSetIndex, inSimulatorNames)) {
        auto dutName = dutDriver->getName();
        auto refName = refDriver->getName();
        auto dutUnit = dutDriver->getUnit();
        auto refUnit = refDriver->getUnit();
        bool res;
        auto channel = dutDriver->addChannel(false, dutName, dutUnit, refName, refUnit, res);
        if (!res) {
            throw std::runtime_error("Error: add channel failed");
        }
        refDriver->addChannel(dutName, refName, channel);
        channel = refDriver->addChannel(true, refName, refUnit, dutName, dutUnit, res);
        if (!res) {
            throw std::runtime_error("Error: add channel failed");
        }
        dutDriver->addChannel(refName, dutName, channel);

        //!< can add Sync Channel between these, it depends on requirements
        // auto dutSimuIndex = dutDriver->getSimulatorSetIndex();
        // auto refSimuIndex = refDriver->getSimulatorSetIndex();
        // auto dutSimuNames = dutDriver->getSimulatorNames();
        // auto refSimuNames = refDriver->getSimulatorNames();

    }

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
                auto sendDutFuture = std::async(std::launch::async, [this] {
                    sendTransaction(false);
                });
                auto sendRefFuture = std::async(std::launch::async, [this] {
                    sendTransaction(true);
                });
                sendDutFuture.wait();
                sendRefFuture.wait();
                incTransPtr();
                auto dutFuture = std::async(std::launch::async, [this, i] {
                    return dutDriver->drivingStep(i == transactions.size() - 1);
                });
                auto refFuture = std::async(std::launch::async, [this, i] {
                    return refDriver->drivingStep(i == transactions.size() - 1);
                });
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

    
} // namespace MLVP::Driver
