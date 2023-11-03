#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "Sequencer/sequencer.h"

namespace MVM {
namespace Transaction {
/* dangerous */
class TransactionCounter
{
private:
    unsigned long long transactionBaseID;
    std::mutex transactionCounterMutex;

    TransactionCounter() : transactionBaseID(0) {}

public:
    TransactionCounter(TransactionCounter const &) = delete;
    void operator=(TransactionCounter const &) = delete;

    static TransactionCounter & getInstance() {
        static TransactionCounter instance;
        return instance;
    }

    unsigned long long getTransactionID() {
        std::lock_guard<std::mutex> lock(transactionCounterMutex);
        return transactionBaseID++;
    }
};

struct TransactionItem
{
    unsigned long long cycles;
    unsigned long long maxCycles;
    MVM::Type::SerialTest inSignal;
    MVM::Type::SerialTest dutOutSignal;
    MVM::Type::SerialTest refOutSignal;
};

class Transaction
{
private:
    unsigned long long transactionID;
    std::mutex transactionMutex;
    TransactionItem transactionItems;

public:
    Transaction() = delete;
    

    Transaction(Transaction const &) = delete;

    Transaction(unsigned long long maxCycles, MVM::Type::SerialTest test);

    void setDutOutSignal(int index, MVM::Type::TestPoint dutOutSignal) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.dutOutSignal[index] = dutOutSignal;
    }

    void setRefOutSignal(int index, MVM::Type::TestPoint refOutSignal) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.refOutSignal[index] = refOutSignal;
    }

    void setCycle(int cycles) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.cycles = cycles;
    }

    unsigned long long getTransactionID() {
        return transactionID;
    }

    const MVM::Type::SerialTest & getInSignal() {
        return transactionItems.inSignal;
    }

    const MVM::Type::SerialTest & getDutOutSignal() {
        return transactionItems.dutOutSignal;
    }

    const MVM::Type::SerialTest & getRefOutSignal() {
        return transactionItems.refOutSignal;
    }

    unsigned long long getCycles() {
        return transactionItems.cycles;
    }

    unsigned long long getMaxCycles() {
        return transactionItems.maxCycles;
    }

    int getTestsSize() {
        return transactionItems.inSignal.size();
    }

    bool compareRefDut(int index) {
        return transactionItems.dutOutSignal[index] == transactionItems.refOutSignal[index];
    }

};

class TransactionLauncher {
private:

public:
    TransactionLauncher() = delete;
    

    static int setupTransaction(unsigned long long maxCycles, std::shared_ptr<MVM::Sequencer::Sequencer> sequencer);

};

    
} // namespace Transaction

    
} // namespace MVM
