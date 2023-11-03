#pragma once

#include <string>
#include <memory>

#include "Driver/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class RefUnitDriver : public MVM::Driver::DriverModel {
protected:
    unsigned long long executeCycles;
    unsigned long long testPtr;
    unsigned long long time;

public:
    RefUnitDriver() = delete;
    ~RefUnitDriver() = default;
    RefUnitDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : MVM::Driver::DriverModel(0, {}, inTransaction), executeCycles(0), testPtr(0), time(0) {}

    /* just execute one test */
    /* no implementation will also be a pure function */
    /* [announcer]|[verifier] must implement */
    // bool drivingStep() override;
};
    
} // namespace RefPack

} // namesapce MVM