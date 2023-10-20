#pragma once

#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class DriverModel {
protected:
    std::shared_ptr<MVM::Transaction::Transaction> transaction;

public:
    DriverModel() = default;
    ~DriverModel() = default;
    DriverModel(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : transaction(inTransaction) {};

    virtual bool drivingStep() = 0;
};
    
} // namespace Driver

    
} // namespace MVM