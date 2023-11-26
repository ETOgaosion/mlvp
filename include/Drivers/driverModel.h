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
    std::string name;
    std::shared_ptr<MLVP::Transaction::Transaction> transaction;
    std::map<std::pair<std::string, std::string>, std::shared_ptr<MLVP::Channel::Channel<DriverModel>>> channels; //! <dest name, Channel<DriverModel>>
    

public:
    DriverModel() = default;
    virtual ~DriverModel() = default;

    explicit DriverModel(std::string inName) : name(std::move(inName)) {
        channels.clear();
    }
    
    virtual std::string getName() {
        return name;
    }

    bool checkTransactionFinish() {
        return transaction->checkTransactionFinish();
    }

    virtual bool setTransaction(std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) {
        if (!checkTransactionFinish()) {
            throw std::runtime_error("Transaction is not finished yet");
            return false;
        }
        transaction = std::move(inTransaction);
        return true;
    }

    std::shared_ptr<MLVP::Transaction::Transaction> getTransaction() {
        return transaction;
    }

    virtual std::shared_ptr<MLVP::Channel::Channel<DriverModel>> addChannel(bool inFromRef, const std::string &inSource, const std::shared_ptr<MLVP::Driver::DriverModel> &inSourceDriver, const std::string &inDestination, const std::shared_ptr<MLVP::Driver::DriverModel> &inDestDriver, bool &res) {
        auto pair = std::make_pair(inSource, inDestination);
        if (channels.count(pair)) {
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
        channels[pair] = std::make_shared<MLVP::Channel::Channel<DriverModel>>(inFromRef, inSource, inSourceDriver, inDestination, inDestDriver);
        res = true;
        return channels[pair];
    }

    virtual bool addChannel(const std::string &inSourceName, const std::string& inDestName, const std::shared_ptr<MLVP::Channel::Channel<DriverModel>>& inChannel) {
        auto pair = std::make_pair(inSourceName, inDestName);
        if (channels.count(pair)) {
            if (MLVP::Library::bugHandleDegree == MLVP::Library::Degree::HIGH) {
                throw std::runtime_error("Channel already exists");
            }
            else {
                return false;
            }
        }
        channels[pair] = inChannel;
        return true;
    }



    virtual std::shared_ptr<MLVP::Channel::Channel<DriverModel>> getChannel(const std::string &inSourceName, const std::string &inDestName, bool &result) {
        if (!channels.count(std::make_pair(inSourceName, inDestName))) {
            result = {};
        }
        return channels[std::make_pair(inSourceName, inDestName)];
    }

    virtual bool drivingStep(bool isLast) = 0;


}; // class DriverModel

} // namespace MLVP::Driver