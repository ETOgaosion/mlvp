#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class RefTransDriver : public DriverModel {
private:
    std::unique_ptr<DriverModel> ref; //!< Actually a RefUnitDriver Child Implemented by user
    int simulatorSetIndex;
    std::vector<std::string> simulatorNames;

public:
    RefTransDriver() = delete;
    ~RefTransDriver() = default;
    RefTransDriver(std::unique_ptr<DriverModel> inRef, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : ref(std::move(inRef)), simulatorSetIndex(inSimulatorSetIndex), simulatorNames(inSimulatorNames) {}

    bool setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) override;

    bool drivingStep() override {
        return ref->drivingStep();
    }

};


} // namespace RefPack
    
} // namespace MVM
