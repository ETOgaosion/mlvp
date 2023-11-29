#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <mutex>
#include <map>
#include <optional>
#include <utility>
#include <functional>

#include "Library/types.h"
#include "Library/error.h"
#include "Evaluator/evaluate.h"

namespace MLVP::Transaction {
/* dangerous */
class TransactionCounter
{
private:
    MLVP::Type::Data transactionBaseID;
    std::mutex transactionCounterMutex;

    TransactionCounter() : transactionBaseID(0) {}

public:
    TransactionCounter(TransactionCounter const &) = delete;
    void operator=(TransactionCounter const &) = delete;

    static TransactionCounter &getInstance() {
        static TransactionCounter instance;
        return instance;
    }

    MLVP::Type::Data getTransactionID() {
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
    MLVP::Type::PortsData inSignal;
    MLVP::Type::PortsData dutOutSignal;
    MLVP::Type::PortsData refOutSignal;

public:
    int cycles;

    TransactionTop() : dutStatus(TransactionReqStatus::NEW), refStatus(TransactionReqStatus::NEW), cycles(0), inSignal({}) {
        dutOutSignal.clear();
        refOutSignal.clear();
    }
    ~TransactionTop() = default;
    explicit TransactionTop(MLVP::Type::PortsData newInSignal) : dutStatus(TransactionReqStatus::NEW), refStatus(TransactionReqStatus::NEW), cycles(0), inSignal(std::move(newInSignal)) {
        dutOutSignal.clear();
        refOutSignal.clear();
    }

    bool isAllDone() {
        return dutStatus == TransactionReqStatus::DONE && refStatus == TransactionReqStatus::DONE;
    }

    bool isAllDone(bool isRef) {
        if (isRef) {
            return refStatus == TransactionReqStatus::DONE;
        }
        else {
            return dutStatus == TransactionReqStatus::DONE;
        }
    }

    void setDone(bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::DONE;
        }
        else {
            dutStatus = TransactionReqStatus::DONE;
        }
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

    bool setInSignal(const std::string &portName, MLVP::Type::Data data) {
        if (inSignal.contains(portName)) {
            if (MLVP::Library::bugHandleDegree != MLVP::Library::Degree::SKIP) {
                throw std::runtime_error("Transaction in signal port name conflict");
            }
            return false;
        }
        inSignal.emplace(portName, data);
        return true;
    }

    MLVP::Type::Data getInSignal(const std::string &portName) {
        if (!inSignal.contains(portName)) {
            throw std::runtime_error("Transaction in signal port name not found");
        }
        return inSignal[portName];
    }

    const MLVP::Type::PortsData &getInSignal() {
        return inSignal;
    }

    const MLVP::Type::PortsData &getOutSignal(bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::HANDLING;
            return refOutSignal;
        }
        else {
            dutStatus = TransactionReqStatus::HANDLING;
            return dutOutSignal;
        }
    }

    MLVP::Type::Data getOutSignal(const std::string &portName, bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::HANDLING;
            if (!refOutSignal.contains(portName)) {
                throw std::runtime_error("Transaction out signal port name not found");
            }
            return refOutSignal[portName];
        }
        else {
            dutStatus = TransactionReqStatus::HANDLING;
            if (!dutOutSignal.contains(portName)) {
                throw std::runtime_error("Transaction out signal port name not found");
            }
            return dutOutSignal[portName];
        }
    }

    void setOutSignal(const MLVP::Type::PortsData &inOutSignal, bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::DONE;
            refOutSignal = inOutSignal;
        }
        else {
            dutStatus = TransactionReqStatus::DONE;
            dutOutSignal = inOutSignal;
        }
    }

    void setOutSignal(const std::string &portName, MLVP::Type::Data data, bool fromRef) {
        if (fromRef) {
            refStatus = TransactionReqStatus::DONE;
            if (refOutSignal.contains(portName)) {
                if (MLVP::Library::bugHandleDegree != MLVP::Library::Degree::SKIP) {
                    throw std::runtime_error("Transaction out signal port name conflict");
                }
                return;
            }
            refOutSignal.emplace(portName, data);
        }
        else {
            dutStatus = TransactionReqStatus::DONE;
            if (dutOutSignal.contains(portName)) {
                if (MLVP::Library::bugHandleDegree != MLVP::Library::Degree::SKIP) {
                    throw std::runtime_error("Transaction out signal port name conflict");
                }
                return;
            }
            dutOutSignal.emplace(portName, data);
        }
    }

    bool compareRefDut() {
        if (MLVP::Evaluator::Evaluator::getInstance().hasValidUserEval("user", "top", true)) {
            return MLVP::Evaluator::Evaluator::getInstance().eval("user", "top", true, dutOutSignal, refOutSignal);
        }
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
    MLVP::Type::PortsData inSignal;

    TransactionReq() = delete;
    ~TransactionReq() = default;

    TransactionReq(int inId, std::string inSrc, std::string inDest) : id(inId), src(std::move(inSrc)), dest(std::move(inDest)), status(TransactionReqStatus::NEW) { inSignal.clear(); }
    TransactionReq(int inId, std::string inSrc, std::string inDest, MLVP::Type::PortsData  newInSignal) : id(inId), src(std::move(inSrc)), dest(std::move(inDest)), status(TransactionReqStatus::NEW), inSignal(std::move(newInSignal)) {}

    bool operator==(const TransactionReq &req) const {
        return id == req.id && src == req.src && dest == req.dest;
    }

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
        resp = std::move(inResp);
    }

    std::shared_ptr<TransactionResp> getResp() {
        return resp;
    }



};

struct TransactionResp {
    int id;
    std::string src;
    std::string dest;
    MLVP::Type::PortsData outSignal;
    std::shared_ptr<TransactionReq> req;

    TransactionResp() = delete;
    TransactionResp(const std::shared_ptr<TransactionReq>&inReq, std::string inSrc, std::string inDest) : req(inReq), id(inReq->id), src(std::move(inSrc)), dest(std::move(inDest)) { outSignal.clear(); }
    TransactionResp(const std::shared_ptr<TransactionReq>&inReq, std::string inSrc, std::string inDest, MLVP::Type::PortsData inOutSignal) : req(inReq), id(inReq->id), src(std::move(inSrc)), dest(std::move(inDest)), outSignal(std::move(inOutSignal)) {}

    bool operator==(const TransactionResp &resp) const {
        return id == resp.id && src == resp.src && dest == resp.dest;
    }

    void setOutSignal(MLVP::Type::PortsData inOutSignal) {
        outSignal = std::move(inOutSignal);
    }

    void setOutSignal(const std::string &portName, MLVP::Type::Data data) {
        if (outSignal.contains(portName)) {
            if (MLVP::Library::bugHandleDegree != MLVP::Library::Degree::SKIP) {
                throw std::runtime_error("Transaction response out signal port name conflict");
            }
            return;
        }
        outSignal.emplace(portName, data);
    }

    MLVP::Type::Data getOutSignal(const std::string &portName) {
        if (!outSignal.contains(portName)) {
            throw std::runtime_error("Transaction response out signal port name not found");
        }
        return outSignal[portName];
    }
};

/**
 * @brief Transaction class, which is used to store the information of a transaction
 * 
 */
class Transaction
{
private:
    MLVP::Type::Data transactionID;
    std::mutex transactionMutex;

public:
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
    std::map<std::pair<std::string, std::string>, std::vector<std::pair<TransactionReq, std::vector<TransactionResp>>>> dutUserTransactions;
    std::map<std::pair<std::string, std::string>, std::vector<std::pair<TransactionReq, std::vector<TransactionResp>>>> refUserTransactions;

    Transaction() = delete;
    ~Transaction() = default;

    explicit Transaction(MLVP::Type::PortsData inTest) : transactionItems(std::move(inTest)), dutUserTransactions({}), refUserTransactions({}) {
        transactionID = TransactionCounter::getInstance().getTransactionID();
    }

    MLVP::Type::Data getTransactionID() const {
        return transactionID;
    }

    // ============================== TransactionItem Methods ==============================
    void setOutSignal(const MLVP::Type::PortsData &inOutSignal, bool fromRef) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.setOutSignal(inOutSignal, fromRef);
    }

    void setOutSignal(const std::string &portName, MLVP::Type::Data data, bool fromRef) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.setOutSignal(portName, data, fromRef);
    }

    void setCycle(int cycles) {
        std::lock_guard<std::mutex> lock(transactionMutex);
        transactionItems.setCycles(cycles);
    }

    const MLVP::Type::PortsData &getInSignal() {
        return transactionItems.getInSignal();
    }

    MLVP::Type::Data getInSignal(const std::string &portName) {
        return transactionItems.getInSignal(portName);
    }

    const MLVP::Type::PortsData &getOutSignal(bool fromRef) {
        return transactionItems.getOutSignal(fromRef);
    }

    MLVP::Type::Data getCycles() const {
        return transactionItems.getCycles();
    }

    // ================================== Request - Response method ====================================

    /**
     * @brief recommended method to add a inter-module request
     * 
     * @param inSrc source module name
     * @param inDest destination module name
     * @param inSignal request port - data signal
     * @param fromRef whether the request is from 0 - dut or 1 - ref
     */
    TransactionReq &addRequest(const std::string &inSrc, const std::string &inDest, const MLVP::Type::PortsData &inSignal, bool fromRef);

    /**
     * @brief recommended method to add a inter-module request
     * 
     * @param inSrc source module name
     * @param inDest destination module name
     * @param fromRef whether the request is from 0 - dut or 1 - ref
     */
    TransactionReq &addRequest(const std::string &inSrc, const std::string &inDest, bool fromRef);

    /**
     * @brief add response to a request
     * 
     * @param req recommand to pass raw request, notice that we use reference only to avoid copy overhead, but we cannot promise that user will use raw request
     * @param outSignal response port - data signal
     * @param fromRef whether the response is from 0 - dut or 1 - ref
     */
    void addResponse(TransactionReq &req, MLVP::Type::PortsData outSignal, bool fromRef, bool burst);

    /**
     * @brief check whether there is a request from src to dest
     * 
     * @param src request src
     * @param dest request dest
     * @param fromRef dut or ref
     * @return true 
     * @return false 
     */
    std::vector<int> checkRequest(const std::string &src, const std::string &dest, bool fromRef);

    /**
     * @brief Get the Request object
     * 
     * @param src 
     * @param dest 
     * @param reqId 
     * @param fromRef 
     * @return const TransactionReq& 
     */
    TransactionReq &getRequest(const std::string &src, const std::string &dest, int reqId, bool fromRef) {
        if (reqId == -1) {
            return fromRef ? refUserTransactions[std::make_pair(src, dest)].back().first : dutUserTransactions[std::make_pair(src, dest)].back().first;
        }
        return fromRef ? refUserTransactions[std::make_pair(src, dest)][reqId].first : dutUserTransactions[std::make_pair(src, dest)][reqId].first;
    }

    /**
     * @brief get request number
     * 
     * @param src 
     * @param dest 
     * @param fromRef 
     * @return int 
     */
    int checkRequestNumber(const std::string &src, const std::string &dest, bool fromRef) {
        return fromRef ? (int)refUserTransactions[std::make_pair(src, dest)].size() : (int)dutUserTransactions[std::make_pair(src, dest)].size();
    }

    /**
     * @brief Get the Response object
     * 
     * @param src 
     * @param dest 
     * @param reqId 
     * @param respId
     * @param fromRef 
     * @return const TransactionResp& 
     */
    TransactionResp &getResponse(const std::string &src, const std::string &dest, int reqId, int respId, bool fromRef) {
        if (reqId == -1) {
            if (respId == -1) {
                return fromRef ? refUserTransactions[std::make_pair(src, dest)].back().second.back() : dutUserTransactions[std::make_pair(src, dest)].back().second.back();
            }
        }
        if (respId == -1) {
            return fromRef ? refUserTransactions[std::make_pair(src, dest)][reqId].second.back() : dutUserTransactions[std::make_pair(src, dest)][reqId].second.back();
        }
        else {
            return fromRef ? refUserTransactions[std::make_pair(dest, src)][reqId].second[respId] : dutUserTransactions[std::make_pair(dest, src)][reqId].second[respId];
        }
    }

    /**
     * @brief Check whether the response exist
     * 
     * @param src 
     * @param dest 
     * @param index 
     * @param fromRef 
     * @return true 
     * @return false 
     */
    bool checkResponseExistence(const std::string &src, const std::string &dest, int index, bool fromRef) {
        return fromRef ? refUserTransactions[std::make_pair(dest, src)][index].second.size() : dutUserTransactions[std::make_pair(dest, src)][index].second.size();
    }

    /**
     * @brief check the dut and ref transaction finished
     * 
     * @return true 
     * @return false 
     */
    bool checkTransactionFinish();

    /**
     * @brief check the dut or ref transaction finished
     * 
     * @param isRef
     * @return true 
     * @return false 
     */
    bool checkTransactionFinish(bool isRef);

    std::vector<std::reference_wrapper<TransactionResp>> getAllTransactionResp(bool fromRef);

    // =========================================== Comparison of dut ref ==============================================

    static bool compareRefDutResponse(TransactionReq &dutReq, TransactionReq &refReq);

    bool compareRefDutResponse(const std::string &src, const std::string &dest, int dutReqId, int refReqId);

    bool compareRefDutResponse(const TransactionReq &req);

    bool compareRefDutResponse();

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

    static int setupTransaction(const std::shared_ptr<MLVP::Type::SerialTestSet> &dataSet);

};

    
} // namespace MLVP::Transaction

    

