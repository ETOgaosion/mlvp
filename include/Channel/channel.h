/**
 * @file channel.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>
#include <future>

#include "Library/types.h"
#include "Transaction/transaction.h"

namespace MLVP::Channel {
/**
 * @brief Channel is a message middleware between components, as a proxy message handler
 * @details All Channels are single way, if you need double way, register a reverse Channel please
 * 
 */
template <class T>
class Channel {
private:
    bool fromRef;
    std::string source;
    std::shared_ptr<T> sourceDriver; //!< source driver, actually a UnitDriver Child
    std::string destination;
    std::shared_ptr<T> destDriver; //!< dest driver, actually a UnitDriver Child
    MLVP::Type::PortsData data;
    std::multimap<std::string, MLVP::Type::Data> multiData;
    std::shared_ptr<MLVP::Transaction::Transaction> transaction;

public:
    Channel() = delete;
    ~Channel() = default;

    Channel(bool inFromRef, std::string inSource, std::shared_ptr<T> inSourceDriver, std::string inDestination, std::shared_ptr<T> inDestDriver) : fromRef(inFromRef), source(std::move(inSource)), sourceDriver(std::move(inSourceDriver)), destination(std::move(inDestination)), destDriver(std::move(inDestDriver)) { data.clear(); }

    void setTransaction(std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) {
        transaction = std::move(inTransaction);
    }

    bool hasData(const std::string &inName) {
        return data.contains(inName);
    }

    void setData(MLVP::Type::PortsData inData, bool toTransaction, bool inIsResponse, bool burst) {
        //!< Notice that merge will not cover the raw elements, so we need to reverse
        if (burst) {
            for (auto &i : inData) {
                multiData.insert(std::make_pair(i.first, i.second));
            }
        }
        else {
            std::swap(data, inData);
            data.merge(inData);
        }
        if (toTransaction) {
            if (inIsResponse) {
                auto &req = transaction->getRequest("source", "dest", -1, fromRef);
                transaction->addResponse(req, inData, fromRef, burst);
            }
            else {
                transaction->addRequest(source, destination, inData, fromRef);
                //!< sync start response thread
                auto responseFuture = std::async(std::launch::async, [this]{
                    destDriver->drivingStep(false);
                });
                //!< wait for response, just stall for some cycles, wait for data income
                // responseFuture.get();
            }
        }
    }

    MLVP::Type::Data getData(const std::string &inName, bool &exist) {
        if (multiData.contains(inName)) {
            exist = true;
            auto startData = multiData.find(inName);
            auto ret = startData->second;
            multiData.erase(startData);
            return ret;
        
        }
        if (data.contains(inName)) {
            exist = true;
            return data[inName];
        }
        exist = false;
        return 0;
    }

};

} // namesapce MLVP::Channel