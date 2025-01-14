/**
 * @file simulator.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <memory>
#include <utility>

#include "Library/types.h"
#include "Drivers/driverModel.h"
#include "Channel/channel.h"
#include "Transaction/transaction.h"

namespace MLVP::Simulator {

typedef std::map<std::tuple<std::string, std::string, bool>, std::shared_ptr<MLVP::Channel::Channel<MLVP::Driver::DriverModel>>> ChannelsType;

class Simulator
{
protected:
    bool connectToRef;
    std::string name;
    ChannelsType &channels; //! <dest name, Channel<DriverModel>>
    std::shared_ptr<MLVP::Transaction::Transaction> transaction;

public:
    Simulator() = delete;
    virtual ~Simulator() = default;

    explicit Simulator(bool inConnectToRef, std::string inName, ChannelsType &inChannels, std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) : connectToRef(inConnectToRef), name(std::move(inName)), channels(inChannels), transaction(std::move(inTransaction)) {}

    virtual void resetChannels() {
        for (auto & it : channels) {
            it.second->reset();
        }
    }

    /**
     * @brief Use return value to check if transaction is done
     * 
     * @return true 
     * @return false 
     */
    virtual bool exec() = 0;
};

}