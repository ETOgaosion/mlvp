#pragma once

#include <memory>

#include "Driver/driverModel.h"
#include "RefPack/Memory/memory.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace RefPack {
class MemoryDriver : public MVM::Driver::DriverModel {
private:
    std::unique_ptr<Memory> top;
    unsigned long executeCycles;
    unsigned long testPtr;
    unsigned long time;

public:
    MemoryDriver() = delete;
    ~MemoryDriver() = default;
    MemoryDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(inTransaction), top(std::make_unique<Memory>()), executeCycles(0), testPtr(0), time(0) {
        top->clk = 0;
        top->reset = 1;
    }

    /* just execute one test */
    bool drivingStep() override;
};
    
} // namespace RefPack



} // namesapce MVM