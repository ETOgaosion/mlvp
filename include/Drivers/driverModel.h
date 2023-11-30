/**
 * @file driverModel.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <functional>

#include "Channel/channel.h"
#include "Transaction/transaction.h"
#include "Library/types.h"
#include "Library/error.h"

namespace MLVP::Driver {
class DriverModel {
protected:
    bool isRef = false;
    std::string name;
    int resetCycles = 0;
    std::shared_ptr<MLVP::Transaction::Transaction> transaction;
    std::map<std::tuple<std::string, std::string, bool>, std::shared_ptr<MLVP::Channel::Channel<DriverModel>>> channels; //! <dest name, Channel<DriverModel>>
    

public:
    DriverModel() = default;
    virtual ~DriverModel() = default;

    explicit DriverModel(int inResetCycles, bool inIsRef, std::string inName) : resetCycles(inResetCycles), isRef(inIsRef), name(std::move(inName)) {
        channels.clear();
    }
    
    virtual std::string getName() {
        return name;
    }

    bool checkTransactionFinish() {
        return !transaction || transaction->checkTransactionFinish();
    }

    virtual bool setTransaction(std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) {
        if (transaction && !checkTransactionFinish()) {
            throw std::runtime_error("Transaction is not finished yet");
            return false;
        }
        transaction = std::move(inTransaction);
        for (auto &channel : channels) {
            channel.second->setTransaction(transaction);
        }
        return true;
    }

    std::shared_ptr<MLVP::Transaction::Transaction> getTransaction() {
        return transaction;
    }

    virtual std::shared_ptr<MLVP::Channel::Channel<DriverModel>> addChannel(const std::string &inSource, const std::shared_ptr<MLVP::Driver::DriverModel> &inSourceDriver, const std::string &inDestination, const std::shared_ptr<MLVP::Driver::DriverModel> &inDestDriver, bool &res) {
        auto tuple = std::make_tuple(inSource, inDestination, isRef);
        if (channels.count(tuple)) {
            if (MLVP::Library::bugHandleDegree == MLVP::Library::Degree::HIGH) {
                throw std::runtime_error("Channel already exists");
            }
            else {
                res = false;
                return {};
            }
        }
        if (inSource != name && inDestination != name) {
            res = false;
            return {};
        }
        channels.emplace(tuple, std::make_shared<MLVP::Channel::Channel<DriverModel>>(isRef, inSource, inSourceDriver, inDestination, inDestDriver));
        res = true;
        return channels[tuple];
    }

    virtual bool addChannel(const std::string &inSourceName, const std::string& inDestName, const std::shared_ptr<MLVP::Channel::Channel<DriverModel>>& inChannel) {
        auto tuple = std::make_tuple(inSourceName, inDestName, inChannel->getFromRef());
        if (channels.count(tuple)) {
            if (MLVP::Library::bugHandleDegree == MLVP::Library::Degree::HIGH) {
                throw std::runtime_error("Channel already exists");
            }
            else {
                return false;
            }
        }
        channels.emplace(tuple, inChannel);
        return true;
    }



    virtual std::shared_ptr<MLVP::Channel::Channel<DriverModel>> getChannel(bool inFromRef, const std::string &inSourceName, const std::string &inDestName, bool &result) {
        if (!channels.count(std::make_tuple(inSourceName, inDestName, inFromRef))) {
            result = false;
        }
        result = true;
        return channels[std::make_tuple(inSourceName, inDestName, inFromRef)];
    }

    virtual void reset() = 0;

    virtual bool drivingStep(bool isLast) = 0;


}; // class DriverModel

} // namespace MLVP::Driver