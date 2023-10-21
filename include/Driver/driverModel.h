#pragma once

#include <string>

#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class DriverModel {
protected:
    int driverID;
    std::shared_ptr<MVM::Transaction::Transaction> transaction;
    std::string logPath;

public:
    DriverModel() = default;
    virtual ~DriverModel() = default;

    DriverModel(int inDriverID, std::string inLogPath, std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : driverID(inDriverID), logPath(inLogPath), transaction(inTransaction) {};

    virtual bool drivingStep() = 0;
};
    
} // namespace Driver

    
} // namespace MVM