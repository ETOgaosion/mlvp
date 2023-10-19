#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include <TestsParser/analyser.h>

namespace MVM {
namespace TransactionLauncher {
/* dangerous */
class TransactionCounter
{
private:
    unsigned long transactionBaseID;
    std::mutex transactionCounterMutex;

    TransactionCounter() : transactionBaseID(0) {}

public:
    TransactionCounter(TransactionCounter const &) = delete;
    void operator=(TransactionCounter const &) = delete;

    static TransactionCounter & getInstance() {
        static TransactionCounter instance;
        return instance;
    }

    unsigned long getTransactionID() {
        std::lock_guard<std::mutex> lock(transactionCounterMutex);
        return transactionBaseID++;
    }
};

struct TransactionItem
{
    std::vector<unsigned long> inSignal;
    std::vector<unsigned long> dutOutSignal;
    std::vector<unsigned long> refOutSignal;
};

class Transaction
{
private:
    unsigned long transactionID;
    std::mutex transactionMutex;
    TransactionItem transactionItems;

public:
    Transaction() = delete;
    ~Transaction() = default;

    Transaction(Transaction const &) = delete;

    Transaction(std::vector<unsigned long> test);

    void setDutOutSignal(std::vector<unsigned long> dutOutSignal) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.dutOutSignal = dutOutSignal;
    }

    void setRefOutSignal(std::vector<unsigned long> refOutSignal) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.refOutSignal = refOutSignal;
    }

};

class TransactionLauncher {
private:

public:
    TransactionLauncher() = delete;
    TransactionLauncher(MVM::TestsParser::Analyser & analyser);
    ~TransactionLauncher() = default;

    /* launch transactions */
    int launchTransactions();

};

    
} // namespace TransactionLauncher

    
} // namespace MVM
