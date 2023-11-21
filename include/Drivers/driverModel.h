#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Transaction/transaction.h"
#include "Library/types.h"

namespace MVM::Driver {
class DriverModel {
protected:
    std::string name;
    std::shared_ptr<MVM::Transaction::Transaction> transaction;

public:
    DriverModel() = default;
    virtual ~DriverModel() = default;

    explicit DriverModel(std::string inName) : name(std::move(inName)) {}
    
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
        transaction = std::move(inTransaction);
        return true;
    }

    std::shared_ptr<MVM::Transaction::Transaction> getTransaction() {
        return transaction;
    }

    virtual bool drivingStep(bool isLast) = 0;


}; // class DriverModel

} // namespace MVM::Driver