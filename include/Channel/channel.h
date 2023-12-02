/**
 * @file channel.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <iterator>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>
#include <future>
#include <iostream>

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
    std::mutex channelMutex;
    std::string source;
    std::shared_ptr<T> sourceDriver; //! source driver, actually a UnitDriver Child
    std::string destination;
    std::shared_ptr<T> destDriver; //! dest driver, actually a UnitDriver Child
    MLVP::Type::PortsData data;
    std::multimap<std::string, MLVP::Type::Data> multiData;
    std::unordered_map<std::string, int> multiDataIndex;
    std::shared_ptr<MLVP::Transaction::Transaction> transaction;
    std::vector<std::future<void>> responseFutures;

public:
    Channel() = delete;
    ~Channel() = default;

    Channel(bool inFromRef, std::string inSource, std::shared_ptr<T> inSourceDriver, std::string inDestination, std::shared_ptr<T> inDestDriver) : fromRef(inFromRef), source(std::move(inSource)), sourceDriver(std::move(inSourceDriver)), destination(std::move(inDestination)), destDriver(std::move(inDestDriver)) {
        data.clear();
        responseFutures.clear();
    }

    void print() {
        std::cout << "Channel: " << source << " -> " << destination << std::endl;
        std::cout << "Data: [";
        for (auto &i : data) {
            std::cout << "{" << i.first << ": " << i.second << "}, ";
        }
        std::cout << "]" << std::endl;
        std::cout << "MultiData: [";
        for (auto &i : multiData) {
            std::cout << "{" << i.first << ": " << i.second << "}, ";
        }
        std::cout << std::endl;
    }

    void reset() {
        data.clear();
        multiData.clear();
        multiDataIndex.clear();
        responseFutures.clear();
    }

    bool getFromRef() { return fromRef; }

    void setTransaction(std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) {
        transaction = std::move(inTransaction);
    }

    bool hasData(const std::string &inName) {
        if (USE_THREADS) {
            channelMutex.lock();
        }
        bool ret = data.contains(inName);
        if (USE_THREADS) {
            channelMutex.unlock();
        }
        return ret;
    }

    void setData(MLVP::Type::PortsData inData, bool toTransaction, bool inIsResponse, bool useThreads) {
        if (USE_THREADS) {
            channelMutex.lock();
        }
        //! Notice that merge will not cover the raw elements, so we need to reverse
        std::swap(data, inData);
        data.merge(inData);
        if (toTransaction) {
            if (inIsResponse) {
                auto &req = transaction->getRequest(source, destination, -1, fromRef);
                transaction->addResponse(req, inData, fromRef);
            }
            else {
                transaction->addRequest(source, destination, inData, fromRef);
                //! sync start response thread
                if (USE_THREADS && useThreads) {
                    channelMutex.unlock();
                    responseFutures.push_back(std::async(std::launch::async, [this]{
                        destDriver->drivingStep(false);
                    }));
                    channelMutex.lock();
                }
                else {
                    if (USE_THREADS) {
                        channelMutex.unlock();
                    }
                    destDriver->drivingStep(false);
                    if (USE_THREADS) {
                        channelMutex.lock();
                    }
                }
                //! wait for response, just stall for some cycles, wait for data income
                // responseFuture.get();
            }
        }
        if (USE_THREADS) {
            channelMutex.unlock();
        }
    }

    /**
     * @brief Set the Data object
     * 
     * @param inData 
     * @param toTransaction whether the data is set to transaction
     * @param inIsResponse whather set transaction response, or request
     * @param burst use burst mode to add multiple data later
     * @param isLast is last element of burst mode
     * @param useThreads use threads to spawn responser, <b>dut can use this, but if you use transaction mode ref, make sure this option is false</b>
     * @param syncMode use sync mode to synchronize data read and write
     */
    void setData(MLVP::Type::PortsData inData, bool toTransaction, bool inIsResponse, bool burst, bool isLast, bool useThreads) {
        if (USE_THREADS) {
            channelMutex.lock();
        }
        //! Notice that merge will not cover the raw elements, so we need to reverse
        if (burst) {
            for (auto &i : inData) {
                multiData.insert(std::make_pair(i.first, i.second));
                if (!multiDataIndex.contains(i.first)) {
                    multiDataIndex[i.first] = 0;
                }
            }
        }
        else {
            std::swap(data, inData);
            data.merge(inData);
        }
        if (toTransaction) {
            if (inIsResponse) {
                auto &req = transaction->getRequest(source, destination, -1, fromRef);
                transaction->addResponse(req, inData, fromRef, burst, isLast);
            }
            else {
                transaction->addRequest(source, destination, inData, fromRef);
                //! sync start response thread
                if (USE_THREADS && useThreads) {
                    channelMutex.unlock();
                    responseFutures.push_back(std::async(std::launch::async, [this]{
                        destDriver->drivingStep(false);
                    }));
                    channelMutex.lock();
                }
                else {
                    if (USE_THREADS) {
                        channelMutex.unlock();
                    }
                    destDriver->drivingStep(false);
                    if (USE_THREADS) {
                        channelMutex.lock();
                    }
                }
                //! wait for response, just stall for some cycles, wait for data income
                // responseFuture.get();
            }
        }
        if (USE_THREADS) {
            channelMutex.unlock();
        }
    }

    /**
     * @brief Get the Data object
     * @details <b>Notice that this fetch function can only deal with one port, if you need to sync multiple ports, use function below</b>
     * @param inName 
     * @param syncMode whether use sync mode to sync data read and write
     * @param exist return value, get whether the data exist
     * @return MLVP::Type::Data 
     */
    MLVP::Type::Data getData(const std::string &inName, bool &exist) {
        if (USE_THREADS) {
            channelMutex.lock();
        }
        exist = false;
        if (multiData.contains(inName)) {
            exist = true;
            auto range = multiData.equal_range(inName);
            int index = 0;
            for (auto it = range.first; it != range.second; ++it) {
                if (index == multiDataIndex[inName]) {
                    multiDataIndex[inName]++;
                    if (USE_THREADS) {
                        channelMutex.unlock();
                    }
                    return it->second;
                }
                else {
                    index++;
                }
            }
            exist = false;
            if (USE_THREADS) {
                channelMutex.unlock();
            }
            return 0;
        
        }
        if (data.contains(inName)) {
            exist = true;
            MLVP::Type::Data ret = data[inName];
            if (USE_THREADS) {
                channelMutex.unlock();
            }
            return ret;
        }
        if (USE_THREADS) {
            channelMutex.unlock();
        }
        return 0;
    }

    MLVP::Type::PortsData getData(const std::vector<std::string> &inNames, bool &exist) {
        if (USE_THREADS) {
            channelMutex.lock();
        }
        exist = false;
        MLVP::Type::PortsData ret;
        for (auto &inName : inNames) {
            ret[inName] = 0;
            if (multiData.contains(inName)) {
                exist = true;
                auto range = multiData.equal_range(inName);
                int index = 0;
                for (auto it = range.first; it != range.second; ++it) {
                    if (index == multiDataIndex[inName]) {
                        multiDataIndex[inName]++;
                        ret[inName] = it->second;
                        break;
                    }
                    else {
                        index++;
                    }
                }
            }
            if (data.contains(inName)) {
                exist = true;
                ret[inName] = data[inName];
            }
        }
        if (USE_THREADS) {
            channelMutex.unlock();
        }
        return ret;
    }

};

} // namesapce MLVP::Channel