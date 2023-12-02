/**
 * @file transaction.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "Transaction/transaction.h"

#include <bits/ranges_algo.h>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "Database/database.h"
#include "Library/error.h"
#include "Library/types.h"

using namespace std;
using namespace MLVP::Transaction;
using namespace MLVP::Database;
using namespace MLVP::Type;
using namespace MLVP::Library;

void Transaction::print(PrintOption option) {
    cout << "Transaction: ";
    cout << "; transactionID: " <<  transactionID << endl;
    if (option == PrintOption::ALL || option == PrintOption::TOP) {
        transactionItems.print();
    }
    cout << endl;
    if (option == PrintOption::ALL || option == PrintOption::DUT) {
        for (auto &transaction : dutUserTransactions) {
            cout << "{" << transaction.first.first << " -> {" << transaction.first.second << ": ";
            for (auto &item : transaction.second) {
                item.first.print();
                for (auto &resp : item.second) {
                    resp.print();
                }
            }
            cout << "}}" << endl;
        }
    }
    if (option == PrintOption::ALL || option == PrintOption::REF) {
        for (auto &transaction : refUserTransactions) {
            cout << "{" << transaction.first.first << " -> {" << transaction.first.second << ": ";
            for (auto &item : transaction.second) {
                item.first.print();
                for (auto &resp : item.second) {
                    resp.print();
                }
            }
            cout << "}}" << endl;
        }
    }
}

TransactionReq &Transaction::addRequest(const string &inSrc, const string &inDest, const PortsData &inSignal, bool fromRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto modulePair = make_pair(inSrc, inDest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (userTransaction.contains(modulePair)) {
        TransactionReq req((int)userTransaction[modulePair].size(), inSrc, inDest, inSignal);
        userTransaction[modulePair].emplace_back(make_pair(req, vector<TransactionResp>({})));
        userTransaction[modulePair].back().first.setHandling();
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        return userTransaction[modulePair].back().first;
    }
    else {
        TransactionReq req(0, inSrc, inDest, inSignal);
        userTransaction.emplace(modulePair, vector<pair<TransactionReq, vector<TransactionResp>>>{make_pair(req, vector<TransactionResp>())});
        userTransaction[modulePair].back().first.setHandling();
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        return userTransaction[modulePair].back().first;
    }
}

TransactionReq &Transaction::addRequest(const string &inSrc, const string &inDest, bool fromRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto modulePair = make_pair(inSrc, inDest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (userTransaction.contains(modulePair)) {
        TransactionReq req((int)userTransaction[modulePair].size(), inSrc, inDest);
        userTransaction[modulePair].emplace_back(make_pair(req, vector<TransactionResp>({})));
        userTransaction[modulePair].back().first.setHandling();
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        return userTransaction[modulePair].back().first;
    }
    else {
        TransactionReq req(0, inSrc, inDest);
        userTransaction.emplace(modulePair, vector<pair<TransactionReq, vector<TransactionResp>>>{make_pair(req, vector<TransactionResp>())});
        userTransaction[modulePair].back().first.setHandling();
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        return userTransaction[modulePair].back().first;
    }
}

void Transaction::addResponse(TransactionReq &req, PortsData outSignal, bool fromRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto modulePair = make_pair(req.src, req.dest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (!userTransaction.contains(modulePair)) {
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        throw runtime_error("Transaction request not found");
    }
    auto &transactionItem = userTransaction[modulePair][req.id];
    TransactionResp resp(make_shared<TransactionReq>(transactionItem.first), req.dest, req.src, std::move(outSignal));
    if (transactionItem.second.empty()) {
        transactionItem.second.emplace_back(resp);
    }
    else {
        transactionItem.second.back() = std::move(resp);
    }
    transactionItem.first.setResp(std::make_shared<TransactionResp>(transactionItem.second.back()));
    transactionItem.first.setDone();
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
}

void Transaction::addResponse(TransactionReq &req, PortsData outSignal, bool fromRef, bool burst, bool isLast) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto modulePair = make_pair(req.src, req.dest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (!userTransaction.contains(modulePair)) {
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        throw runtime_error("Transaction request not found");
    }
    auto &transactionItem = userTransaction[modulePair][req.id];
    TransactionResp resp(make_shared<TransactionReq>(transactionItem.first), req.dest, req.src, std::move(outSignal));
    if (burst) {
        transactionItem.second.emplace_back(resp);
    }
    else {
        if (transactionItem.second.empty()) {
            transactionItem.second.emplace_back(resp);
        }
        else {
            transactionItem.second.back() = std::move(resp);
        }
    }
    transactionItem.first.setResp(std::make_shared<TransactionResp>(transactionItem.second.back()));
    if (!burst) {
        transactionItem.first.setDone();
    }
    else {
        if (isLast) {
            transactionItem.first.setDone();
        }
    }
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
}

void Transaction::setRequestDone(TransactionReq &req, bool fromRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto modulePair = make_pair(req.src, req.dest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (!userTransaction.contains(modulePair)) {
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        throw runtime_error("Transaction request not found");
    }
    auto &transactionItem = userTransaction[modulePair][req.id];
    transactionItem.first.setDone();
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
}

vector<int> Transaction::checkRequest(const string &src, const string &dest, bool fromRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto modulePair = make_pair(src, dest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (!userTransaction.contains(modulePair)) {
        if (USE_THREADS) {
            transactionMutex.unlock();
        }
        throw runtime_error("Transaction request not found");
    }
    vector<int> res;
    for (int i = 0; i < userTransaction[modulePair].size(); i++) {
        if (!userTransaction[modulePair][i].first.isDone()) {
            res.emplace_back(i);
        }
    }
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
    return res;
}

bool Transaction::checkTransactionFinish() {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto res = dutTotalStatus == TransactionStatus::DONE && refTotalStatus == TransactionStatus::DONE;
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
    return res;
}

bool Transaction::checkTransactionFinish(bool isRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    auto res = isRef ? refTotalStatus == TransactionStatus::DONE : dutTotalStatus == TransactionStatus::DONE;
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
    return res;
}

vector<reference_wrapper<TransactionResp>> Transaction::getAllTransactionResp(bool fromRef) {
    if (USE_THREADS) {
        transactionMutex.lock();
    }
    vector<reference_wrapper<TransactionResp>> transactionResp;
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    for (auto &transaction : userTransaction) {
        for (auto &transactionItems : transaction.second) {
            if (transactionItems.second.empty()) {
                if (USE_THREADS) {
                    transactionMutex.unlock();
                }
                throw runtime_error("Transaction response not found");
            }
            else {
                ranges::all_of(transactionItems.second, [&transactionResp](TransactionResp &resp) {
                    transactionResp.emplace_back(resp);
                    return true;
                });
            }
        }
    }
    if (USE_THREADS) {
        transactionMutex.unlock();
    }
    return transactionResp;
}

bool Transaction::compareRefDutResponse(TransactionReq &dutReq, TransactionReq &refReq) {
    if (!dutReq.getResp() || !refReq.getResp()) {
        throw runtime_error("Transaction response not found");
    }
    if (MLVP::Evaluator::Evaluator::getInstance().hasValidUserEval(dutReq.src, dutReq.dest, true)) {
        return MLVP::Evaluator::Evaluator::getInstance().eval(dutReq.src, dutReq.dest, true, dutReq.inSignal, dutReq.getResp()->outSignal);
    }
    else {
        return dutReq.getResp()->outSignal == refReq.getResp()->outSignal;
    }
}

bool Transaction::compareRefDutResponse(const string &src, const string &dest, int dutReqId, int refReqId) {
    auto modulePair = std::make_pair(src, dest);
    if (!dutUserTransactions.contains(modulePair) || !refUserTransactions.contains(modulePair)) {
        throw runtime_error("Transaction request not found");
    }
    if (dutUserTransactions[modulePair][dutReqId].second.empty() || refUserTransactions[modulePair][refReqId].second.empty()) {
        throw runtime_error("Transaction response not found");
    }
    if (MLVP::Evaluator::Evaluator::getInstance().hasValidUserEval(src, dest, true)) {
        for (int i = 0; i < dutUserTransactions[modulePair][dutReqId].second.size(); i++) {
            if (!MLVP::Evaluator::Evaluator::getInstance().eval(src, dest, true, dutUserTransactions[modulePair][dutReqId].first.inSignal, dutUserTransactions[modulePair][dutReqId].second[i].outSignal)) {
                return false;
            }
        }
        return true;
    }
    else {
        for (int i = 0; i < dutUserTransactions[modulePair][dutReqId].second.size(); i++) {
            if (dutUserTransactions[modulePair][dutReqId].second[i].outSignal != refUserTransactions[modulePair][refReqId].second[i].outSignal) {
                return false;
            }
        }
        return true;
    }
}

bool Transaction::compareRefDutResponse(const TransactionReq &req) {
    auto modulePair = std::make_pair(req.src, req.dest);
    if (!dutUserTransactions.contains(modulePair) || !refUserTransactions.contains(modulePair)) {
        throw runtime_error("Transaction request not found");
    }
    if (dutUserTransactions[modulePair][req.id].second.empty() || refUserTransactions[modulePair][req.id].second.empty()) {
        throw runtime_error("Transaction response not found");
    }
    if (MLVP::Evaluator::Evaluator::getInstance().hasValidUserEval(req.src, req.dest, true)) {
        for (int i = 0; i < dutUserTransactions[modulePair][req.id].second.size(); i++) {
            if (!MLVP::Evaluator::Evaluator::getInstance().eval(req.src, req.dest, true, dutUserTransactions[modulePair][req.id].first.inSignal, dutUserTransactions[modulePair][req.id].second[i].outSignal)) {
                return false;
            }
        }
        return true;
    }
    else {
        for (int i = 0; i < dutUserTransactions[modulePair][req.id].second.size(); i++) {
            if (dutUserTransactions[modulePair][req.id].second[i].outSignal != refUserTransactions[modulePair][req.id].second[i].outSignal) {
                return false;
            }
        }
        return true;
    }
}

bool Transaction::compareRefDutResponse() {
    for (auto &transaction : dutUserTransactions) {
        for (int i = 0; i < transaction.second.size(); i++) {
            if (!compareRefDutResponse(transaction.first.first, transaction.first.second, i, i)) {
                transaction.second[i].first.print();
                for (auto &resp : transaction.second[i].second) {
                    resp.print();
                    cout << endl;
                }
                throw runtime_error("transactionItems check failed " + to_string(i) + " " + transaction.first.first + " " + transaction.first.second);
                return false;
            }
        }
    }
    return true;
}

bool Transaction::compareRefDut(CompareOption type) {
    bool res = true;
    switch (type)
    {
    case CompareOption::ALL:
        res &= transactionItems.compareRefDut();
        if (!res) {
            transactionItems.print();
            cout << endl;
            throw runtime_error("transactionItems check failed");
            return res;
        }
        res &= compareRefDutResponse();
        break;
    case CompareOption::INTER_MODULE:
        res &= compareRefDutResponse();
        break;
    case CompareOption::TOP:
        res &= transactionItems.compareRefDut();
        break;
    
    default:
        break;
    }
    return res;
}

/**
 * @brief Set up transaction database
 * 
 * @param dataSet a vector<unordered_map<string, vector<SerialData>>> object
 * @example dataSet:
 * ```txt
 * { SerialTest 1, SerialTest 2, ... }
 * SerialTest:
 * {
 *   { "port1", { 1, 2, 3, 4, 5 } },
 *   { "port2", { 1, 2, 3, 4, 5 } },
 * }
 * ```
 * We need to convert to:
 * ```
 * { TransactionSet 1, TransactionSet 2, ... }
 * TransactionSet:
 * {
 *   { Transaction 1, Transaction 2, ... },
 *   { Transaction 1, Transaction 2, ... },
 * }
 * ```
 * Transaction:
 * {
 *   { "port1", 1 },
 *   { "port2", 1 },
 * }
 * @return int 
 */
int TransactionLauncher::setupTransaction(const shared_ptr<SerialTestSet> &dataSet) {
    vector<shared_ptr<Transaction>> transactions;
    for (auto &serialTest : *dataSet) {
        shared_ptr<PortsData> portsData;
        auto serialTestSize = serialTest.begin()->second.size();
        for (int i = 0; i < serialTestSize; i++) {
            portsData = make_shared<PortsData>();
            for (auto &port : serialTest) {
                portsData->emplace(port.first, port.second[i]);
            }
            transactions.push_back(make_shared<Transaction>(*portsData));
        }
        TransactionDatabase::getInstance().addTransaction(transactions);
    }
    return (int)dataSet->size();
}
