#include "TransactionLauncher/transaction.h"
#include "Database/database.h"

using namespace std;
using namespace MVM::TransactionLauncher;
using namespace MVM::Database;
using namespace MVM::TestsParser;

Transaction::Transaction(std::vector<unsigned long> test) {
    std::lock_guard<std::mutex> lock(transactionMutex);
    transactionID = TransactionCounter::getInstance().getTransactionID();
    transactionItems.inSignal = test;
}

TransactionLauncher::TransactionLauncher(MVM::TestsParser::Analyser & analyser) {
    auto tests = analyser.getTests();
    for (auto & test : tests) {
        auto transactionPtr = make_shared<Transaction>(test);
        TransactionDatabase::getInstance().addTransaction(transactionPtr);
    }
}

int TransactionLauncher::launchTransactions() {
}