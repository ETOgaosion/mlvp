#pragma once

#include <memory>

#include "Driver/driverModel.h"
#include "RefPack/Memory/memoryDriver.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace RefPack {
class RefDriver : public MVM::Driver::DriverModel {
private:
    MemoryDriver memoryDriver;

public:
    RefDriver() = delete;
    ~RefDriver() = default;
    RefDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : memoryDriver(inTransaction) {}

    bool drivingStep() override {
        return memoryDriver.drivingStep();
    }

};



} // namespace RefPack
    
} // namespace MVM
