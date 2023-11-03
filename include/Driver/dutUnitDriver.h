#pragma once

#include <string>
#include <memory>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Driver/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class DutUnitDriver : public MVM::Driver::DriverModel {
protected:
    const std::unique_ptr<VerilatedContext> contextp;
    const std::unique_ptr<VerilatedVcdC> tfp;
    unsigned long long executeCycles;
    unsigned long long testPtr;
    
public:
    DutUnitDriver() = delete;
    virtual ~DutUnitDriver() = default;

    DutUnitDriver(int inDriverID, std::string inLogPath, std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(inDriverID, inLogPath + "/Driver" + std::to_string(inDriverID), inTransaction), contextp(std::make_unique<VerilatedContext>()), tfp(std::make_unique<VerilatedVcdC>()), executeCycles(0), testPtr(0) {
        Verilated::mkdir(inLogPath.c_str());
        Verilated::mkdir(logPath.c_str());
        Verilated::traceEverOn(true);
        contextp->traceEverOn(true);
    }

    /* just execute one test */
    /* no implementation will also be a pure function */
    /* [announcer] must implement */
    // virtual bool drivingStep() = 0;
};

} // namespace Driver

    
} // namespace MVM