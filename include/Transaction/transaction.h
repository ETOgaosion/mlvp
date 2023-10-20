#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include <Sequencer/sequencer.h>
#include <Sequencer/testcases.h>

namespace MVM {
namespace Transaction {
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
    unsigned long cycles;
    unsigned long maxCycles;
    MVM::Sequencer::TestCase inSignal;
    MVM::Sequencer::TestCase dutOutSignal;
    MVM::Sequencer::TestCase refOutSignal;
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

    Transaction(unsigned long maxCycles, MVM::Sequencer::TestCase test);

    void setDutOutSignal(int index, std::vector<unsigned long> dutOutSignal) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.dutOutSignal[index] = dutOutSignal;
    }

    void setRefOutSignal(int index, std::vector<unsigned long> refOutSignal) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.refOutSignal[index] = refOutSignal;
    }

    void setCycle(int cycles) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.cycles = cycles;
    }

    unsigned long getTransactionID() {
        return transactionID;
    }

    const MVM::Sequencer::TestCase & getInSignal() {
        return transactionItems.inSignal;
    }

    const MVM::Sequencer::TestCase & getDutOutSignal() {
        return transactionItems.dutOutSignal;
    }

    const MVM::Sequencer::TestCase & getRefOutSignal() {
        return transactionItems.refOutSignal;
    }

    unsigned long getCycles() {
        return transactionItems.cycles;
    }

    unsigned long getMaxCycles() {
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
    ~TransactionLauncher() = delete;

    static int launchTransaction(MVM::Sequencer::Sequencer & sequencer);

};

    
} // namespace Transaction

    
} // namespace MVM
