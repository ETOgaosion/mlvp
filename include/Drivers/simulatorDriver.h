#pragma once

#include "Drivers/driverModel.h"
#include "Simulator/simulator.h"

namespace MVM::Driver {
class SimulatorDriver : public DriverModel {
private:
    std::unique_ptr<MVM::Simulator::Simulator> simulator;

public:
    SimulatorDriver() = delete;
    virtual ~SimulatorDriver() = default;
    SimulatorDriver(std::string inSimulatorName, std::unique_ptr<MVM::Simulator::Simulator> inSimulator) : DriverModel(inSimulatorName), simulator(std::move(inSimulator)) {}

    /**
     * bool drivingStep() override
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

    static SimulatorlDriverRegistrar& getInstance() {
        static SimulatorlDriverRegistrar instance;
        return instance;
    }

    int registerSimulatorDriver(std::vector<std::pair<std::shared_ptr<SimulatorDriver>, std::shared_ptr<SimulatorDriver>>> inDriverModels) {
        driverModels.push_back(std::unordered_map<std::string, std::pair<std::shared_ptr<SimulatorDriver>, std::shared_ptr<SimulatorDriver>>>());
        for (auto& driver : inDriverModels) {
            driverModels.back()[driver.first->getName()] = driver;
        }
        return driverModels.size() - 1;
    }

    std::shared_ptr<SimulatorDriver> getSimulatorDriver(int index, bool fromRef, std::string inSimulatorName) {
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

} // namespace MVM::Driver