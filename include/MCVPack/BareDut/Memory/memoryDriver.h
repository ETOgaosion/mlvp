#pragma once

#include <memory>
#include <verilated.h>

#include "Driver/driverModel.h"
#include "MCVPack/BareDut/Memory/Vmemory.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace MCVPack {
class MemoryDriver : public MVM::Driver::DriverModel {
private:
    const std::unique_ptr<VerilatedContext> contextp;
    const std::unique_ptr<Vmemory> top;
    unsigned long executeCycles;
    unsigned long testPtr;
    
public:
    MemoryDriver() = delete;
    ~MemoryDriver() = default;

    MemoryDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(inTransaction), contextp(new VerilatedContext), top(new Vmemory{contextp.get(), "top"}), executeCycles(0), testPtr(0) {
        top->clk = 0;
        top->reset = 1;
    }

    /* just execute one test */
    bool drivingStep() override;
};

} // namespace MCVPack

    
} // namespace MVM