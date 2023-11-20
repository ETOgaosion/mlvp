#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class DutUnitDriver : public MVM::Driver::DriverModel {
protected:
    const std::unique_ptr<VerilatedContext> contextp;
    const std::unique_ptr<VerilatedVcdC> tfp;
    std::string logPath;
    
public:
    DutUnitDriver() = delete;
    virtual ~DutUnitDriver() = default;

    DutUnitDriver(std::string inUnitName, int inDriverID, std::string inLogPath) : DriverModel(inUnitName), logPath(inLogPath + "/Driver" + std::to_string(inDriverID)), contextp(std::make_unique<VerilatedContext>()), tfp(std::make_unique<VerilatedVcdC>()) {
        std::filesystem::create_directories(logPath);
        Verilated::traceEverOn(true);
        contextp->traceEverOn(true);
    }

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

    MVM::Type::uint64 syncSignal(std::string portName) override {
        return dut->syncSignal(portName);
    }

    bool drivingStep() override {
        while (!transaction->checkTransactionFinish()) {
            dut->drivingStep();
            for (auto simulatorName : simulatorNames) {
                SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, false, simulatorName)->drivingStep();
            }
        }
        return true;
    }

};

    
} // namespace Driver


} // namespace MVM