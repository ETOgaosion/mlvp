/**
 * @file ref.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <string>
#include <map>
#include <memory>
#include <utility>

#include "Channel/channel.h"
#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"

namespace MLVP::RefPack {

typedef std::map<std::tuple<std::string, std::string, bool>, std::shared_ptr<MLVP::Channel::Channel<MLVP::Driver::DriverModel>>> ChannelsType;

/**
 * @brief Use only when you want to implement a ref driven by cycles, transaction handler logic implemented in refUnitDriver is OK
 * 
 */
class Ref
{
private:
    std::string name;

protected:
    ChannelsType &channels; //! <dest name, Channel<DriverModel>>
    std::shared_ptr<MLVP::Transaction::Transaction> transaction;

public:
    Ref() = delete;
    virtual ~Ref() = default;

    Ref(std::string inName, ChannelsType &inChannels, std::shared_ptr<MLVP::Transaction::Transaction> inTransaction) : name(std::move(inName)), channels(inChannels), transaction(std::move(inTransaction)) {}

    virtual bool exec() = 0;
};

} // namespace MLVP::RefPack
