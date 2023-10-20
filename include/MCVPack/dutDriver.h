#pragma once

#include <memory>

#include "Driver/driverModel.h"
#include "MCVPack/BareDut/Memory/memoryDriver.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace MCVPack {
class DutDriver : public MVM::Driver::DriverModel {
private:
    MemoryDriver memoryDriver;

public:
    DutDriver() = delete;
    ~DutDriver() = default;
    DutDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : memoryDriver(inTransaction) {}

    bool drivingStep() override {
        return memoryDriver.drivingStep();
    }

};

    
} // namespace MCVPack


} // namespace MVM