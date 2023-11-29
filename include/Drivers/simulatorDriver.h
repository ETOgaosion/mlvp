/**
 * @file simulatorDriver.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <utility>

#include "Drivers/driverModel.h"
#include "Simulator/simulator.h"

namespace MLVP::Driver {
class SimulatorDriver : public DriverModel {
private:
    std::shared_ptr<MLVP::Simulator::Simulator> simulator;

public:
    SimulatorDriver() = delete;
    ~SimulatorDriver() override = default;
    SimulatorDriver(int inResetCycles, bool isRef, std::string inSimulatorName, std::shared_ptr<MLVP::Simulator::Simulator> inSimulator) : DriverModel(inResetCycles, isRef, std::move(inSimulatorName)), simulator(std::move(inSimulator)) {}

    /**
     * bool drivingStep(bool isLast) override
     * @brief Driving step for simulator
     * @details just execute one test
     *          no implementation will also be a pure function
     *          [announcer]|[verifier] must implement
     * 
     */


}; // class SimulatorDriver

class SimulatorlDriverRegistrar {
private:
    std::vector<std::unordered_map<std::string, std::pair<std::shared_ptr<SimulatorDriver>, std::shared_ptr<SimulatorDriver>>>> driverModels;
    SimulatorlDriverRegistrar() = default;

public:
    ~SimulatorlDriverRegistrar() = default;

    static SimulatorlDriverRegistrar &getInstance() {
        static SimulatorlDriverRegistrar instance;
        return instance;
    }

    int registerSimulatorDriver(const std::vector<std::pair<std::shared_ptr<SimulatorDriver>, std::shared_ptr<SimulatorDriver>>> &inDriverModels) {
        driverModels.emplace_back();
        for (auto &driver : inDriverModels) {
            driverModels.back().emplace(driver.first->getName(), driver);
        }
        return (int)driverModels.size() - 1;
    }

    std::shared_ptr<SimulatorDriver> getSimulatorDriver(int index, bool fromRef, const std::string &inSimulatorName) {
        if (driverModels.size() <= index) {
            throw std::runtime_error("SimulatorDriver not found");
        }
        if (fromRef) {
            return driverModels[index][inSimulatorName].second;
        }
        else {
            return driverModels[index][inSimulatorName].first;
        }
    }


}; // class SimulatorlRegistrar

} // namespace MLVP::Driver