#pragma once

#include <string>
#include <memory>
#include <verilated.h>

#include "Driver/driverModel.h"
#include "MCVPack/BareDut/Memory/Vmemory.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace MCVPack {
class DutMemoryDriver : public MVM::Driver::DriverModel {
private:
    const std::unique_ptr<VerilatedContext> contextp;
    const std::unique_ptr<Vmemory> top;
    unsigned long executeCycles;
    unsigned long testPtr;
    
public:
    DutMemoryDriver() = delete;
    ~DutMemoryDriver() = default;

    DutMemoryDriver(int inDriverID, std::string inLogPath, std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(inDriverID, inLogPath, inTransaction), contextp(std::make_unique<VerilatedContext>()), top(std::make_unique<Vmemory>(contextp.get(), "top")), executeCycles(0), testPtr(0) {
        Verilated::mkdir(inLogPath.c_str());
        contextp->debug(0);
        contextp->randReset(2);
        contextp->traceEverOn(true);
        top->clk = 0;
        top->reset = 1;
    }

    /* just execute one test */
    bool drivingStep() override;
};

} // namespace MCVPack

    
} // namespace MVM