#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <mutex>
#include <map>
#include <optional>
#include <utility>

#include "Library/types.h"
#include "Evaluator/evaluate.h"

namespace MVM::Transaction {
/* dangerous */
class TransactionCounter
{
private:
    MVM::Type::uint64 transactionBaseID;
    std::mutex transactionCounterMutex;

    TransactionCounter() : transactionBaseID(0) {}

public:
    TransactionCounter(TransactionCounter const &) = delete;
    void operator=(TransactionCounter const &) = delete;

    static TransactionCounter & getInstance() {
        static TransactionCounter instance;
        return instance;
    }

    MVM::Type::uint64 getTransactionID() {
        std::lock_guard<std::mutex> lock(transactionCounterMutex);
        return transactionBaseID++;
    }
};

enum class TransactionReqStatus {
    NEW,
    HANDLING,
    DONE
};

class TransactionTop {
private:
    TransactionReqStatus dutStatus;
    TransactionReqStatus refStatus;
    MVM::Type::SerialTestSingle inSignal;
    MVM::Type::SerialTestSingle dutOutSignal;
    MVM::Type::SerialTestSingle refOutSignal;

public:
    int cycles;
    TransactionTop() = delete;
    ~TransactionTop() = default;
    explicit TransactionTop(MVM::Type::SerialTestSingle newInSignal) : dutStatus(TransactionReqStatus::NEW), refStatus(TransactionReqStatus::NEW), cycles(0), inSignal(std::move(newInSignal)) {
        dutOutSignal.clear();
        refOutSignal.clear();
    }

    bool isAllDone() {
        return dutStatus == TransactionReqStatus::DONE && refStatus == TransactionReqStatus::DONE;
    }

    int getCycles() const {
        return cycles;
    }

    void incCycles() {
        cycles++;
    }

    void setCycles(int inCycles) {
        cycles = inCycles;
    }

    const MVM::Type::SerialTestSingle & getInSignal() {
        return inSignal;
    }

    const MVM::Type::SerialTestSingle & getOutSignal(bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::HANDLING;
            return refOutSignal;
        }
        else {
            dutStatus = TransactionReqStatus::HANDLING;
            return dutOutSignal;
        }
    }

    void setDutOutSignal(const MVM::Type::SerialTestSingle& inOutSignal, bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::DONE;
            refOutSignal = inOutSignal;
        }
        else {
            dutStatus = TransactionReqStatus::DONE;
            dutOutSignal = inOutSignal;
        }
    }

    bool compareRefDut() {
        return dutOutSignal == refOutSignal;
    }
};

struct TransactionResp;

class TransactionReq {
private:
    TransactionReqStatus status;
    std::shared_ptr<TransactionResp> resp;

public:
    int id;
    std::string src;
    std::string dest;
    MVM::Type::SerialTestSingle inSignal;

    ~TransactionReq() = default;
    TransactionReq(int inId, std::string inSrc, std::string inDest, const MVM::Type::SerialTestSingle&) : id(inId), src(std::move(inSrc)), dest(std::move(inDest)), status(TransactionReqStatus::NEW), inSignal(inSignal) {}

    bool isNew() {
        return status == TransactionReqStatus::NEW;
    }

    bool isDone() {
        return status == TransactionReqStatus::DONE;
    }

    void setHandling() {
        if (status != TransactionReqStatus::NEW) {
            throw std::runtime_error("Transaction request not new");
        }
        status = TransactionReqStatus::HANDLING;
    }

    void setDone() {
        if (status != TransactionReqStatus::HANDLING) {
            throw std::runtime_error("Transaction request not handling");
        }
        status = TransactionReqStatus::DONE;
    }

    void setResp(std::shared_ptr<TransactionResp> inResp) {
        resp = inResp;
    }

    std::shared_ptr<TransactionResp> getResp() {
        return resp;
    }

};

struct TransactionResp {
    int id;
    std::string src;
    std::string dest;
    MVM::Type::SerialTestSingle outSignal;
    std::shared_ptr<TransactionReq> req;

    TransactionResp() = delete;
    TransactionResp(std::shared_ptr<TransactionReq> inReq, std::string inSrc, std::string inDest, MVM::Type::SerialTestSingle inOutSignal) : id(inReq->id), src(inSrc), dest(inDest), outSignal(inOutSignal) {}
};

/**
 * @brief Transaction class, which is used to store the information of a transaction
 * 
 */
class Transaction
{
private:
    MVM::Type::uint64 transactionID;
    std::mutex transactionMutex;

    /**
     * @brief Top level transaction interface, no interact with other modules
     * 
     */
    TransactionTop transactionItems;

    /**
     * @brief Transaction Requests and Responses, Here is simplified contents, only include used ones
     * @details Notice that we introduce a new concept *Channel*, which is used to connect the requesters and responsers
     *          basically Channel type is:
     *              std::pair<std::pair<std::string, std::string>, std::vector<std::pair<TransactionReq, std::optional<TransactionResp>>>>
     *          We organize the channels as a hash map
     *          We support multiple requests and responses in a channel
     * 
     *          must use references to avoid much memory copy costs, except in self-target add methods
     * @example <from module, to module> -> <request, response>
     * @todo consider whether make transaction transfering function independent as Channel midware
     * 
     */
    std::map<std::pair<std::string, std::string>, std::vector<std::pair<TransactionReq, std::optional<TransactionResp>>>> dutUserTransactions;
    std::map<std::pair<std::string, std::string>, std::vector<std::pair<TransactionReq, std::optional<TransactionResp>>>> refUserTransactions;

public:
    Transaction() = delete;
    ~Transaction() = default;

    Transaction(MVM::Type::SerialTestSingle inTest) : transactionItems(inTest), dutUserTransactions({}), refUserTransactions({}) {
        transactionID = TransactionCounter::getInstance().getTransactionID();
    }

    /**
     * @brief recommended method to add a inter-module request
     * 
     * @param inSrc source module name
     * @param inDest destination module name
     * @param inSignal request port - data signal
     * @param fromRef whether the request is from 0 - dut or 1 - ref
     */
    const TransactionReq & addRequest(std::string inSrc, std::string inDest, MVM::Type::SerialTestSingle inSignal, bool fromRef);

    /**
     * @brief add response to a request
     * 
     * @param req recommand to pass raw request, notice that we use reference only to avoid copy overhead, but we cannot promise that user will use raw request
     * @param outSignal response port - data signal
     * @param fromRef whether the response is from 0 - dut or 1 - ref
     */
    void addResponse(TransactionReq &req, MVM::Type::SerialTestSingle outSignal, bool fromRef);

    /**
     * @brief get all unhandled requests
     * @param fromRef
     * @return
     */
    const std::vector<TransactionReq> &getUnhandledRequests(bool fromRef);

    std::optional<TransactionReq> getOneRequest(bool fromRef, bool &valid);

    bool checkTransactionFinish();

    const std::vector<TransactionResp> & getAllTransactionResp(bool fromRef);

    void setOutSignal(MVM::Type::SerialTestSingle inOutSignal, bool fromRef) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.setDutOutSignal(inOutSignal, fromRef);
    }

    void setCycle(int cycles) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.setCycles(cycles);
    }

    MVM::Type::uint64 getTransactionID() {
        return transactionID;
    }

    const MVM::Type::SerialTestSingle & getInSignal() {
        return transactionItems.getInSignal();
    }

    const MVM::Type::SerialTestSingle & getOutSignal(bool fromRef) {
        return transactionItems.getOutSignal(fromRef);
    }

    MVM::Type::uint64 getCycles() {
        return transactionItems.getCycles();
    }

    bool compareRefDutResponse(TransactionReq &dutReq, TransactionReq &refReq) {
        if (!dutReq.getResp() || !refReq.getResp()) {
            throw std::runtime_error("Transaction response not found");
        }
        if (MVM::Evaluator::Evaluator::getInstance().hasValidUserEval(dutReq.src, dutReq.dest, true)) {
            return MVM::Evaluator::Evaluator::getInstance().eval(dutReq.src, dutReq.dest, true, dutReq.inSignal, dutReq.getResp()->outSignal);
        }
        else {
            return dutReq.getResp()->outSignal == refReq.getResp()->outSignal;
        }
    }

    bool compareRefDutResponse(std::string src, std::string dest, int reqId) {
        auto modulePair = std::make_pair(src, dest);
        if (!dutUserTransactions.contains(modulePair) || !refUserTransactions.contains(modulePair)) {
            throw std::runtime_error("Transaction request not found");
        }
        if (!dutUserTransactions[modulePair][reqId].second.has_value() || !refUserTransactions[modulePair][reqId].second.has_value()) {
            throw std::runtime_error("Transaction response not found");
        }
        if (MVM::Evaluator::Evaluator::getInstance().hasValidUserEval(src, dest, true)) {
            return MVM::Evaluator::Evaluator::getInstance().eval(src, dest, true, dutUserTransactions[modulePair][reqId].first.inSignal, dutUserTransactions[modulePair][reqId].second.value().outSignal);
        }
        else {
            return dutUserTransactions[modulePair][reqId].second.value().outSignal == refUserTransactions[modulePair][reqId].second.value().outSignal;
        }
    }

    bool compareRefDutResponse(const TransactionReq &req) {
        auto modulePair = std::make_pair(req.src, req.dest);
        if (!dutUserTransactions.contains(modulePair) || !refUserTransactions.contains(modulePair)) {
            throw std::runtime_error("Transaction request not found");
        }
        if (!dutUserTransactions[modulePair][req.id].second.has_value() || !refUserTransactions[modulePair][req.id].second.has_value()) {
            throw std::runtime_error("Transaction response not found");
        }
        if (MVM::Evaluator::Evaluator::getInstance().hasValidUserEval(req.src, req.dest, true)) {
            return MVM::Evaluator::Evaluator::getInstance().eval(req.src, req.dest, true, dutUserTransactions[modulePair][req.id].first.inSignal, dutUserTransactions[modulePair][req.id].second.value().outSignal);
        }
        else {
            return dutUserTransactions[modulePair][req.id].second.value().outSignal == refUserTransactions[modulePair][req.id].second.value().outSignal;
        }
    }

    bool compareRefDutResponse() {
        for (auto &transaction : dutUserTransactions) {
            for (int i = 0; i < transaction.second.size(); i++) {
                if (!compareRefDutResponse(transaction.first.first, transaction.first.second, i)) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * @brief compare the dut and ref response
     * 
     * @param type response index
     *              - 2 for all
     *              - 1 for all inter-modules response
     *              - 0 for top response
     * @return true same
     * @return false not same
     */
    bool compareRefDut(int type);

};

class TransactionLauncher {
private:

public:
    TransactionLauncher() = delete;

    static int setupTransaction(std::shared_ptr<MVM::Type::SerialTestSet> dataSet);

};

    
} // namespace MVM::Transaction

    

