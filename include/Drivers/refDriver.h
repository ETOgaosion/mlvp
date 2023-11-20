#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"
#include "Drivers/dutDriver.h"

namespace MVM {
namespace Driver {
class RefUnitDriver : public MVM::Driver::DriverModel {
public:
    RefUnitDriver() = delete;
    ~RefUnitDriver() = default;
    RefUnitDriver(std::string inUnitName) : DriverModel(inUnitName) {}

    // MVM::Type::uint64 syncSignal(std::string portName) override
    
    /**
     * bool drivingStep() override
     * @brief Driving step for simulator
     * @details just execute one test
     *          no implementation will also be a pure function
     *          [announcer]|[verifier] must implement
     * 
     */
};

class RefTransDriver : public DriverModel {
private:
    std::unique_ptr<DriverModel> ref; //!< Actually a RefUnitDriver Child Implemented by user
    int simulatorSetIndex;
    std::vector<std::string> simulatorNames;
    std::shared_ptr<DriverModel> dutTransDriver; //!< Actually a DutTransDriver

public:
    RefTransDriver() = delete;
    ~RefTransDriver() = default;
    RefTransDriver(std::unique_ptr<DriverModel> inRef, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames, std::shared_ptr<DriverModel> inDutTransDriver) : ref(std::move(inRef)), simulatorSetIndex(inSimulatorSetIndex), simulatorNames(inSimulatorNames), dutTransDriver(inDutTransDriver) {}

    bool setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) override;

    bool drivingStep() override {
        while (!transaction->checkTransactionFinish()) {
            ref->drivingStep();
            for (auto simulatorName : simulatorNames) {
                SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, true, simulatorName)->drivingStep();
            }
        }
        return true;
    }

};


} // namespace RefPack
    
} // namespace MVM
