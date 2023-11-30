/**
 * @file transDriverModel.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <string>
#include <vector>

#include "Drivers/driverModel.h"
#include "Drivers/simulatorDriver.h"
#include "Transaction/transaction.h"

namespace MLVP::Driver {
class TransDriver : public DriverModel {
private:
    bool isRef;
    std::shared_ptr<DriverModel> unit; //! Actually a UnitDriver Child Implemented by user
    std::unordered_map<std::string, std::shared_ptr<DriverModel>> simulatorDrivers; //! Actually a SimulatorDriver Child Implemented by user

public:
    TransDriver() = delete;
    ~TransDriver() override = default;

    /**
     * @brief Construct a new Trans Driver object
     * 
     * @param in class inherited from UnitDriver
     * @param inSimulatorDrivers must be new object, different with simulatorDrivers in RefUnitDriver
     */
    TransDriver(int inResetCycles, bool inIsRef, std::shared_ptr<DriverModel> in, std::unordered_map<std::string, std::shared_ptr<DriverModel>> inSimulatorDrivers) : isRef(inIsRef), unit(std::move(in)), simulatorDrivers(std::move(inSimulatorDrivers)) {
        for (auto &simulatorDriver : simulatorDrivers) {
            bool res;
            //! unitUnit: unitUnit -> simulator
            auto channel = unit->addChannel(unit->getName(), unit, simulatorDriver.first, simulatorDriver.second, res);
            if (!res) {
                throw std::runtime_error("Error: add channel failed");
            }
            //! simulator: unitUnit -> simulator
            simulatorDriver.second->addChannel(unit->getName(), simulatorDriver.first, channel);
            //! simulator: simulator -> unitUnit
            channel = simulatorDriver.second->addChannel(simulatorDriver.first, simulatorDriver.second, unit->getName(), unit, res);
            if (!res) {
                throw std::runtime_error("Error: add channel failed");
            }
            //! unitUnit: simulator -> unitUnit
            unit->addChannel(simulatorDriver.first, unit->getName(), channel);
        }
    }

    std::string getName() override { return unit->getName(); }

    std::shared_ptr<DriverModel> getUnit() { return unit; }

    std::shared_ptr<MLVP::Channel::Channel<DriverModel>> addChannel(const std::string &inSource, const std::shared_ptr<MLVP::Driver::DriverModel> &inSourceDriver, const std::string &inDestination, const std::shared_ptr<MLVP::Driver::DriverModel> &inDestDriver, bool &res) override {
        auto ret = inSourceDriver->addChannel(inSource, inSourceDriver, inDestination, inDestDriver, res);
        if (!res) {
            throw std::runtime_error("Error: add channel failed " + inSource + " " + inDestination);
        }
        return ret;
    }

    bool addChannel(const std::string &inSourceName, const std::string& inDestName, const std::shared_ptr<MLVP::Channel::Channel<DriverModel>>& inChannel) override {
        bool res = true;
        res = unit->addChannel(inSourceName, inDestName, inChannel);
        if (!res) {
            for (auto &simulatorDriver : simulatorDrivers) {
                res = simulatorDriver.second->addChannel(inSourceName, inDestName, inChannel);
                if (res) {
                    break;
                }
            }
            if (!res) {
                throw std::runtime_error("Error: add channel failed");
            }
        }
        return res;
    }

    std::shared_ptr<MLVP::Channel::Channel<DriverModel>> getChannel(bool inIsRef, const std::string &inSourceName, const std::string &inDestName, bool &res) override {
        auto ret = unit->getChannel(inIsRef, inSourceName, inDestName, res);
        if (res) {
            return ret;
        }
        else {
            for (auto &simulatorDriver : simulatorDrivers) {
                ret = simulatorDriver.second->getChannel(inIsRef, inSourceName, inDestName, res);
                if (res) {
                    return ret;
                }
            }
        }
        return ret;
    }

    std::shared_ptr<MLVP::Channel::Channel<DriverModel>> getChannel(const std::string &inSourceName, const std::string &inDestName, bool &res) {
        auto ret = unit->getChannel(isRef, inSourceName, inDestName, res);
        if (res) {
            return ret;
        }
        else {
            for (auto &simulatorDriver : simulatorDrivers) {
                ret = simulatorDriver.second->getChannel(isRef, inSourceName, inDestName, res);
                if (res) {
                    return ret;
                }
            }
        }
        return ret;
    }

    bool setTransaction(std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) override {
        transaction = inTransaction;
        unit->setTransaction(inTransaction);
        for (auto &simulatorDriver : simulatorDrivers) {
            simulatorDriver.second->setTransaction(inTransaction);
        }
        return true;
    }

    void reset() override {
        unit->reset();
        for (auto &simulatorDriver : simulatorDrivers) {
            simulatorDriver.second->reset();
        }
    }

    bool drivingStep(bool isLast) override {
        while (!transaction->checkTransactionFinish(isRef)) {
            unit->drivingStep(isLast);
        }
        return true;
    }

    bool transactionFinish() {
        return transaction->checkTransactionFinish();
    }

};

} // namespace MLVP::Driver