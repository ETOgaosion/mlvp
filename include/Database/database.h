#pragma once

#include <memory>

#include "Transaction/transaction.h"

namespace MVM {
namespace Database {
class TransactionDatabase
{
private:
    std::vector<std::shared_ptr<MVM::Transaction::Transaction>> transactions;

    TransactionDatabase() = default;

public:
    

    TransactionDatabase(TransactionDatabase const &) = delete;
    void operator=(TransactionDatabase const &) = delete;

    static TransactionDatabase & getInstance() {
        static TransactionDatabase instance;
        return instance;
    }

    void addTransaction(std::shared_ptr<MVM::Transaction::Transaction> transaction) {
        transactions.push_back(transaction);
    }

    std::shared_ptr<MVM::Transaction::Transaction> getTransaction(int index) {
        return transactions[index];
    }

    int getTransactionSize() {
        return transactions.size();
    }
};

} // namespace Database
    
} // namespace MVM