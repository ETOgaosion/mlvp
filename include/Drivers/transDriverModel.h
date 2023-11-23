/**
 * @file transDriverModel.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "Drivers/driverModel.h"
#include "Drivers/simulatorDriver.h"
#include "Transaction/transaction.h"
#include <vector>

namespace MLVP::Driver {
class TransDriver : public DriverModel {
private:
    std::shared_ptr<DriverModel> unit; //!< Actually a UnitDriver Child Implemented by user
    int simulatorSetIndex;
    std::vector<std::string> simulatorNames;

public:
    TransDriver() = delete;
    ~TransDriver() override = default;

    /**
     * @brief Construct a new  Trans Driver object
     * 
     * @param in class inherited from UnitDriver
     * @param inSimulatorDrivers must be new object, different with simulatorDrivers in RefUnitDriver
     */
    TransDriver(std::unique_ptr<DriverModel> in, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : unit(std::move(in)), simulatorSetIndex(inSimulatorSetIndex), simulatorNames(std::move(inSimulatorNames)) {
        for (auto &simulatorName : simulatorNames) {
            auto simuDriver = SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(inSimulatorSetIndex, false, simulatorName);
            bool res;
            //!< unitUnit: unitUnit -> simulator
            auto channel = unit->addChannel(false, unit->getName(), unit, simulatorName, simuDriver, res);
            if (!res) {
                throw std::runtime_error("Error: add channel failed");
            }
            //!< simulator: unitUnit -> simulator
            simuDriver->addChannel(unit->getName(), simulatorName, channel);
            //!< simulator: simulator -> unitUnit
            channel = simuDriver->addChannel(false, simulatorName, simuDriver, unit->getName(), unit, res);
            //!< unitUnit: simulator -> unitUnit
            unit->addChannel(simulatorName, unit->getName(), channel);
            if (!res) {
                throw std::runtime_error("Error: add channel failed");
            }
        }
    }

    std::string getName() override { return unit->getName(); }

    [[nodiscard]] int getSimulatorSetIndex() const { return simulatorSetIndex; }

    [[nodiscard]] const std::vector<std::string> &getSimulatorNames() const { return simulatorNames; }

    std::shared_ptr<DriverModel> getUnit() { return unit; }

    std::shared_ptr<MLVP::Channel::Channel<DriverModel>> addChannel(bool inFromRef, const std::string &inSource, const std::shared_ptr<MLVP::Driver::DriverModel> &inSourceDriver, const std::string &inDestination, const std::shared_ptr<MLVP::Driver::DriverModel> &inDestDriver, bool &res) override {
        auto ret = inSourceDriver->addChannel(inFromRef, inSource, inSourceDriver, inDestination, inDestDriver, res);
        if (!res) {
            throw std::runtime_error("Error: add channel failed");
        }
        ret = inDestDriver->addChannel(inFromRef, inSource, inSourceDriver, inDestination, inDestDriver, res);
        if (!res) {
            throw std::runtime_error("Error: add channel failed");
        }
        return ret;
    }

    bool addChannel(const std::string &inSourceName, const std::string& inDestName, const std::shared_ptr<MLVP::Channel::Channel<DriverModel>>& inChannel) override {
        bool res = true;
        res &= unit->addChannel(inSourceName, inDestName, inChannel);
        if (!res) {
            throw std::runtime_error("Error: add channel failed");
            return false;
        }
        res &= SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, false, inDestName)->addChannel(inSourceName, inDestName, inChannel);
        if (!res) {
            throw std::runtime_error("Error: add channel failed");
        }
        return res;
    }

    std::shared_ptr<MLVP::Channel::Channel<DriverModel>> getChannel(const std::string &inSourceName, const std::string &inDestName, bool &res) override {
        auto ret = unit->getChannel(inSourceName, inDestName, res);
        if (res) {
            return ret;
        }
        else {
            for (auto &simulatorName : simulatorNames) {
                ret = SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, false, simulatorName)->getChannel(inSourceName, inDestName, res);
                if (res) {
                    return ret;
                }
            }
        }
        return ret;
    }

    bool setTransaction(std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) override;

    bool drivingStep(bool isLast) override {
        while (!transaction->checkTransactionFinish()) {
            unit->drivingStep(isLast);
        }
        return true;
    }

};

} // namespace MLVP::Driver