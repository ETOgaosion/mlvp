#pragma once

#include <string>
#include <memory>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Driver/driverModel.h"
#include "MCVPack/BareDut/Memory/Vmemory.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace MCVPack {
class DutMemoryDriver : public MVM::Driver::DriverModel {
private:
    const std::unique_ptr<VerilatedContext> contextp;
    const std::unique_ptr<Vmemory> top;
    const std::unique_ptr<VerilatedVcdC> tfp;
    unsigned long long executeCycles;
    unsigned long long testPtr;
    
public:
    DutMemoryDriver() = delete;
    ~DutMemoryDriver() = default;

    DutMemoryDriver(int inDriverID, std::string inLogPath, std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(inDriverID, inLogPath + "/Driver" + std::to_string(inDriverID), inTransaction), contextp(std::make_unique<VerilatedContext>()), top(std::make_unique<Vmemory>(contextp.get(), "top")), tfp(std::make_unique<VerilatedVcdC>()), executeCycles(0), testPtr(0) {
        Verilated::mkdir(inLogPath.c_str());
        Verilated::mkdir(logPath.c_str());
        Verilated::traceEverOn(true);
        contextp->debug(0);
        contextp->randReset(2);
        contextp->traceEverOn(true);
        top->clk = 1;
        top->trace(tfp.get(), 99);
        tfp->open((logPath + "/memory.vcd").c_str());
    }

    /* just execute one test */
    bool drivingStep() override;
};

} // namespace MCVPack

    
} // namespace MVM