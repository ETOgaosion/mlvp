#pragma once

#include <string>
#include <memory>

#include "Driver/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace RefPack {
template <class T>
class RefDriver : public MVM::Driver::DriverModel {
private:
    std::unique_ptr<MVM::Driver::DriverModel> ref;

public:
    RefDriver() = delete;
    ~RefDriver() = default;
    RefDriver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : ref(std::make_unique<T>(inTransaction)) {}

    bool drivingStep() override {
        return ref->drivingStep();
    }

};



} // namespace RefPack
    
} // namespace MVM
