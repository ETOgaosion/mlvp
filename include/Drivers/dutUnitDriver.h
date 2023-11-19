#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <verilated.h>
#include <verilated_vcd_c.h>

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

    /**
     * bool drivingStep() override
     * @brief Driving step for simulator
     * @details just execute one test
     *          no implementation will also be a pure function
     *          [announcer]|[verifier] must implement
     * 
     */
};

} // namespace Driver

    
} // namespace MVM