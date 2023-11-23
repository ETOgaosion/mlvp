/**
 * @file database.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <memory>

#include "Transaction/transaction.h"

namespace MLVP::Database {
class TransactionDatabase
{
private:
    std::vector<std::vector<std::shared_ptr<MLVP::Transaction::Transaction>>> transactions;

    TransactionDatabase() = default;

public:
    TransactionDatabase(TransactionDatabase const &) = delete;
    void operator=(TransactionDatabase const &) = delete;

    static TransactionDatabase &getInstance() {
        static TransactionDatabase instance;
        return instance;
    }

    void addTransaction(const std::vector<std::shared_ptr<MLVP::Transaction::Transaction>> &transaction) {
        transactions.push_back(transaction);
    }

    std::vector<std::shared_ptr<MLVP::Transaction::Transaction>> getTransaction(int index) {
        return transactions[index];
    }

    int getTransactionSize() {
        return (int)transactions.size();
    }
};

} // namespace MLVP