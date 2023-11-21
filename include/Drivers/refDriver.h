#pragma once

#include <string>
#include <utility>
#include <utility>
#include <vector>
#include <memory>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"
#include "Drivers/dutDriver.h"
#include "Drivers/simulatorDriver.h"

namespace MVM::Driver {
class RefUnitDriver : public MVM::Driver::DriverModel {
public:
    RefUnitDriver() = delete;
    ~RefUnitDriver() override = default;
    explicit RefUnitDriver(std::string inUnitName) : DriverModel(std::move(inUnitName)) {}

    //!< MVM::Type::Data syncSignal(std::string portName) override
    
    /**
     * bool drivingStep(bool isLast) override
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

public:
    RefTransDriver() = delete;
    ~RefTransDriver() override = default;
    RefTransDriver(std::unique_ptr<DriverModel> &inRef, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : ref(std::move(inRef)), simulatorSetIndex(inSimulatorSetIndex), simulatorNames(std::move(inSimulatorNames)) {}

    bool setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) override;

    bool drivingStep(bool isLast) override {
        while (!transaction->checkTransactionFinish()) {
            ref->drivingStep(isLast);
            for (const auto &simulatorName : simulatorNames) {
                SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, true, simulatorName)->drivingStep(isLast);
            }
        }
        return true;
    }

};


} // namespace MVM
