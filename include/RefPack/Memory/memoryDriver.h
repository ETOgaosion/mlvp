#pragma once

#include <string>
#include <memory>

#include "Driver/driverModel.h"
#include "RefPack/Memory/memory.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace RefPack {
class RefMemoryDriver : public MVM::Driver::DriverModel {
private:
    std::unique_ptr<Memory> top;
    unsigned long long executeCycles;
    unsigned long long testPtr;
    unsigned long long time;

public:
    RefMemoryDriver() = delete;
    ~RefMemoryDriver() = default;
    RefMemoryDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(0, {}, inTransaction), top(std::make_unique<Memory>()), executeCycles(0), testPtr(0), time(0) {
        top->clk = 1;
    }

    /* just execute one test */
    bool drivingStep() override;
};
    
} // namespace RefPack



} // namesapce MVM