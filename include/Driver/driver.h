#pragma once

#include <memory>
#include "Driver/driverModel.h"
#include "Transaction/transaction.h"
#include "MCVPack/dutDriver.h"
#include "RefPack/refDriver.h"

namespace MVM {
namespace Driver {
class A {};

class B : public A {};

class Driver {
private:
    std::unique_ptr<DriverModel> dutDriver;
    std::unique_ptr<DriverModel> refDriver;
    std::shared_ptr<MVM::Transaction::Transaction> transaction;

public:
    ~Driver() = default;
    Driver(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : transaction(inTransaction), dutDriver(std::make_unique<MVM::MCVPack::DutDriver>(inTransaction)), refDriver(std::make_unique<MVM::RefPack::RefDriver>(inTransaction)) {}

    int driving(std::string &errorMsg);


};

    
} // namespace Driver

    
} // namespace MVM
