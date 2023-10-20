#include "Transaction/transaction.h"
#include "Database/database.h"

using namespace std;
using namespace MVM::Transaction;
using namespace MVM::Database;
using namespace MVM::Sequencer;

Transaction::Transaction(unsigned long maxCycles, MVM::Sequencer::TestCase test) {
    transactionID = TransactionCounter::getInstance().getTransactionID();
    transactionItems.cycles = 0;
    transactionItems.maxCycles = maxCycles;
    transactionItems.inSignal = test;
    transactionItems.dutOutSignal.resize(test.size());
    transactionItems.refOutSignal.resize(test.size());
}

int TransactionLauncher::launchTransaction(MVM::Sequencer::Sequencer & sequencer) {
    auto tests = sequencer.getTests();
    for (auto & test : tests) {
        auto transactionPtr = make_shared<Transaction>(test);
        TransactionDatabase::getInstance().addTransaction(transactionPtr);
    }
    return tests.size();
}