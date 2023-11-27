/**
 * @file transaction.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Transaction/transaction.h"

#include <bits/ranges_algo.h>
#include <functional>
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

TransactionReq &Transaction::addRequest(const string &inSrc, const string &inDest, const PortsData &inSignal, bool fromRef) {
    lock_guard<mutex> lock(transactionMutex);
    auto modulePair = make_pair(inSrc, inDest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (userTransaction.contains(modulePair)) {
        TransactionReq req((int)userTransaction[modulePair].size(), inSrc, inDest, inSignal);
        userTransaction[modulePair].emplace_back(make_pair(req, vector<TransactionResp>({})));
        return userTransaction[modulePair].back().first;
    }
    else {
        TransactionReq req(0, inSrc, inDest, inSignal);
        userTransaction.emplace(modulePair, vector<pair<TransactionReq, vector<TransactionResp>>>{make_pair(req, vector<TransactionResp>())});
        return userTransaction[modulePair].back().first;
    }
}

TransactionReq &Transaction::addRequest(const string &inSrc, const string &inDest, bool fromRef) {
    lock_guard<mutex> lock(transactionMutex);
    auto modulePair = make_pair(inSrc, inDest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (userTransaction.contains(modulePair)) {
        TransactionReq req((int)userTransaction[modulePair].size(), inSrc, inDest);
        userTransaction[modulePair].emplace_back(make_pair(req, vector<TransactionResp>({})));
        return userTransaction[modulePair].back().first;
    }
    else {
        TransactionReq req(0, inSrc, inDest);
        userTransaction.emplace(modulePair, vector<pair<TransactionReq, vector<TransactionResp>>>{make_pair(req, vector<TransactionResp>())});
        return userTransaction[modulePair].back().first;
    }
}

void Transaction::addResponse(TransactionReq &req, PortsData outSignal, bool fromRef, bool burst) {
    lock_guard<mutex> lock(transactionMutex);
    auto modulePair = make_pair(req.src, req.dest);
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    if (!userTransaction.contains(modulePair)) {
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
    transactionItem.first.setDone();
}

bool Transaction::checkTransactionFinish() {
        bool finished = transactionItems.isAllDone();
        if (finished) {
            if (dutUserTransactions.size() != refUserTransactions.size()) {
                return false;
            }
            else {
                for (auto &dutTrans : dutUserTransactions) {
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

vector<reference_wrapper<TransactionResp>> Transaction::getAllTransactionResp(bool fromRef) {
    vector<reference_wrapper<TransactionResp>> transactionResp;
    auto &userTransaction = fromRef ? refUserTransactions : dutUserTransactions;
    for (auto &transaction : userTransaction) {
        for (auto &transactionItems : transaction.second) {
            if (transactionItems.second.empty()) {
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
    return transactionResp;
}

bool Transaction::compareRefDutResponse(TransactionReq &dutReq, TransactionReq &refReq) {
    if (!dutReq.getResp() || !refReq.getResp()) {
        throw std::runtime_error("Transaction response not found");
    }
    if (MLVP::Evaluator::Evaluator::getInstance().hasValidUserEval(dutReq.src, dutReq.dest, true)) {
        return MLVP::Evaluator::Evaluator::getInstance().eval(dutReq.src, dutReq.dest, true, dutReq.inSignal, dutReq.getResp()->outSignal);
    }
    else {
        return dutReq.getResp()->outSignal == refReq.getResp()->outSignal;
    }
}

bool Transaction::compareRefDutResponse(const std::string &src, const std::string &dest, int dutReqId, int refReqId) {
    auto modulePair = std::make_pair(src, dest);
    if (!dutUserTransactions.contains(modulePair) || !refUserTransactions.contains(modulePair)) {
        throw std::runtime_error("Transaction request not found");
    }
    if (dutUserTransactions[modulePair][dutReqId].second.empty() || refUserTransactions[modulePair][refReqId].second.empty()) {
        throw std::runtime_error("Transaction response not found");
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
        throw std::runtime_error("Transaction request not found");
    }
    if (dutUserTransactions[modulePair][req.id].second.empty() || refUserTransactions[modulePair][req.id].second.empty()) {
        throw std::runtime_error("Transaction response not found");
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
                return false;
            }
        }
    }
    return true;
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


int TransactionLauncher::setupTransaction(const shared_ptr<SerialTestSet> &dataSet) {
    vector<shared_ptr<Transaction>> transactions;
    transactions.reserve(dataSet->size());
    for (int i = 0; i < dataSet->begin()->size(); i++) {
        PortsData portsData;
        for (auto &test : *dataSet) {
            for (auto &port : test) {
                portsData.emplace(port.first, port.second[i]);
            }
        }
        transactions.push_back(make_shared<Transaction>(portsData));
    }
    TransactionDatabase::getInstance().addTransaction(transactions);
    return (int)dataSet->size();
}
