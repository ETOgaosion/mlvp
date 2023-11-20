#pragma once

#include <string>
#include <unordered_map>

#include "Transaction/transaction.h"
#include "Library/types.h"

namespace MVM {
namespace Driver {
class DriverModel {
protected:
    std::string name;
    std::shared_ptr<MVM::Transaction::Transaction> transaction;

public:
    DriverModel() = default;
    virtual ~DriverModel() = default;

    DriverModel(std::string inName) : name(inName) {}
    
    std::string getName() {
        return name;
    }

    bool checkTransactionFinish() {
        return transaction->checkTransactionFinish();
    }

    virtual bool setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) {
        if (!checkTransactionFinish()) {
            throw std::runtime_error("Transaction is not finished yet");
            return false;
        }
        transaction = inTransaction;
        return true;
    }

    std::shared_ptr<MVM::Transaction::Transaction> getTransaction() {
        return transaction;
    }

    virtual bool drivingStep() = 0;

    virtual MVM::Type::uint64 syncSignal(std::string portName) { throw std::runtime_error("Not implemented"); return 0; }

    virtual MVM::Type::uint64 syncSignal(std::string simulatorName, std::string portName) { throw std::runtime_error("Not implemented"); return 0; }


}; // class DriverModel


} // namespace Driver


} // namespace MVM