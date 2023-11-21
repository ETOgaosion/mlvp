#pragma once

#include <memory>

#include "Transaction/transaction.h"

namespace MVM::Database {
class TransactionDatabase
{
private:
    std::vector<std::vector<std::shared_ptr<MVM::Transaction::Transaction>>> transactions;

    TransactionDatabase() = default;

public:
    TransactionDatabase(TransactionDatabase const &) = delete;
    void operator=(TransactionDatabase const &) = delete;

    static TransactionDatabase &getInstance() {
        static TransactionDatabase instance;
        return instance;
    }

    void addTransaction(const std::vector<std::shared_ptr<MVM::Transaction::Transaction>> &transaction) {
        transactions.push_back(transaction);
    }

    std::vector<std::shared_ptr<MVM::Transaction::Transaction>> getTransaction(int index) {
        return transactions[index];
    }

    int getTransactionSize() {
        return (int)transactions.size();
    }
};

} // namespace MVM