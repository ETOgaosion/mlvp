#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class DutTransDriver : public DriverModel {
private:
    std::unique_ptr<DriverModel> dut; //!< Actually a DutUnitDriver Child Implemented by user
    int simulatorSetIndex;
    std::vector<std::string> simulatorNames;

public:
    DutTransDriver() = delete;
    ~DutTransDriver() = default;

    /**
     * @brief Construct a new Dut Trans Driver object
     * 
     * @param inDut class inherited from DutUnitDriver
     * @param inSimulatorDrivers must be new object, different with simulatorDrivers in RefUnitDriver
     */
    DutTransDriver(std::unique_ptr<DriverModel> inDut, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : dut(std::move(inDut)), simulatorSetIndex(inSimulatorSetIndex), simulatorNames(inSimulatorNames) {}

    bool setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) override;

    bool drivingStep() override {
        bool ret = false;
        while (!transaction->checkTransactionFinish()) {
            ret = dut->drivingStep();
            for (auto simulatorName : simulatorNames) {
                SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, simulatorName)->drivingStep();
            }
        }
        return ret;
    }

};

    
} // namespace Driver


} // namespace MVM