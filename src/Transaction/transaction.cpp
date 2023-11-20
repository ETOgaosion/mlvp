#include "Transaction/transaction.h"
#include "Database/database.h"
#include "Library/error.h"

using namespace std;
using namespace MVM::Transaction;
using namespace MVM::Database;
using namespace MVM::Type;
using namespace MVM::Library;

const TransactionReq & Transaction::addRequest(string inSrc, string inDest, SerialTestSingle inSignal, bool fromRef) {
    lock_guard<mutex> lock(transactionMutex);
    auto modulePair = make_pair(inSrc, inDest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (userTransaction.contains(modulePair)) {
        TransactionReq req(userTransaction[modulePair].size(), inSrc, inDest, inSignal);
        userTransaction[modulePair].push_back(make_pair(req, nullopt));
        return userTransaction[modulePair].back().first;
    }
    else {
        TransactionReq req(0, inSrc, inDest, inSignal);
        userTransaction[modulePair] = vector<pair<TransactionReq, optional<TransactionResp>>>{make_pair(req, nullopt)};
        return userTransaction[modulePair].back().first;
    }
}

void Transaction::addResponse(TransactionReq &req, SerialTestSingle outSignal, bool fromRef) {
    lock_guard<mutex> lock(transactionMutex);
    auto modulePair = make_pair(req.src, req.dest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (!userTransaction.contains(modulePair)) {
        throw runtime_error("Transaction request not found");
    }
    auto &transactionItem = userTransaction[modulePair][req.id];
    TransactionResp resp(make_shared<TransactionReq>(transactionItem.first), req.dest, req.src, outSignal);
    transactionItem.second = make_optional(move(resp));
    transactionItem.first.setResp(std::make_shared<TransactionResp>(transactionItem.second.value()));
    transactionItem.first.setDone();
}

const vector<TransactionReq> & Transaction::getUnhandledRequests(bool fromRef) {
    vector<TransactionReq> unhandledRequest;
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    for (auto & transaction : userTransaction) {
        for (auto & transactionItem : transaction.second) {
            if (transactionItem.first.isNew()) {
                unhandledRequest.push_back(transactionItem.first);
            }
        }
    }
    return unhandledRequest;
}

optional<TransactionReq> Transaction::getOneRequest(bool fromRef, bool &valid) {
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    for (auto & transaction : userTransaction) {
        for (auto &transactionItem : transaction.second) {
            if (transactionItem.first.isNew()) {
                valid = true;
                return make_optional(transactionItem.first);
            }
        }
    }
    valid = false;
    return nullopt;
}

bool Transaction::checkTransactionFinish() {
        bool finished = transactionItems.isAllDone();
        if (finished) {
            if (dutUserTransactions.size() != refUserTransactions.size()) {
                return false;
            }
            else {
                for (auto & dutTrans : dutUserTransactions) {
                    if (!refUserTransactions.contains(dutTrans.first) || dutTrans.second.size() != refUserTransactions[dutTrans.first].size()) {
                        return false;
                    }
                    else {
                        for (int i = 0; i < dutTrans.second.size(); i++) {
                            if (!dutTrans.second[i].first.isDone() || !refUserTransactions[dutTrans.first][i].first.isDone()) {
                                return false;
                            }
                        }
                        return true;
                    }
                }
            }
        }
        return false;
}

const vector<TransactionResp> & Transaction::getAllTransactionResp(bool fromRef) {
    vector<TransactionResp> transactionResp;
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    for (auto & transaction : userTransaction) {
        for (auto & transactionItems : transaction.second) {
            if (!transactionItems.second.has_value()) {
                throw runtime_error("Transaction response not found");
            }
            else {
                transactionResp.push_back(transactionItems.second.value());
            }
        }
    }
    return transactionResp;
}

bool Transaction::compareRefDut(int type) {
    bool res = true;
    switch (type)
    {
    case 2:
        res &= transactionItems.compareRefDut();
        if (!res) {
            return res;
        }
        res &= compareRefDutResponse();
        break;
    case 1:
        res &= compareRefDutResponse();
        break;
    case 0:
        res &= transactionItems.compareRefDut();
        break;
    
    default:
        break;
    }
    return res;
}


int TransactionLauncher::setupTransaction(vector<SerialTestSingle> dataSet) {
    vector<shared_ptr<Transaction>> transactions;
    for (auto & test : dataSet) {
        transactions.push_back(make_shared<Transaction>(test));
    }
    TransactionDatabase::getInstance().addTransaction(transactions);
    return dataSet.size();
}
