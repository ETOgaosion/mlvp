#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <map>
#include <utility>

#include "Library/types.h"

namespace MVM::Channel {
/**
 * @brief Channel is used if you do not want some data to be passed through transaction, and add a direct connection between two components' ports
 * 
 */
class Channel {
private:
    std::string source;
    std::string destination;
    std::unordered_map<std::string, MVM::Type::Data> data;

public:
    Channel() = delete;
    ~Channel() = default;

    Channel(std::string inSource, std::string inDestination) : source(std::move(inSource)), destination(std::move(inDestination)) { data.clear(); }

    bool hasData(const std::string &inName) {
        return data.contains(inName);
    }

    bool setData(const std::string &inName, MVM::Type::Data inData) {
        if (data.contains(inName)) {
            data[inName] = inData;
            return true;
        }
        return false;
    }

    MVM::Type::Data getData(const std::string &inName) {
        if (data.contains(inName)) {
            return data[inName];
        }
        return 0;
    }

};

class ChannelRegistrar {
private:
    std::map<std::tuple<std::string, std::string, bool>, Channel> channelMap;

    ChannelRegistrar() = default;

public:
    ~ChannelRegistrar() = default;

    static ChannelRegistrar &getInstance() {
        static ChannelRegistrar instance;
        return instance;
    }

    bool registerChannel(const std::string &inSource, const std::string &inDestination, bool fromRef) {
        if (channelMap.contains(std::make_tuple(inSource, inDestination, fromRef))) {
            return false;
        }
        channelMap[std::make_tuple(inSource, inDestination, fromRef)] = Channel(inSource, inDestination);
        return true;
    }

    bool hasChannel(const std::string &inSource, const std::string &inDestination, bool fromRef) {
        return channelMap.contains(std::make_tuple(inSource, inDestination, fromRef));
    }

    bool hasChannelData(const std::string &inSource, const std::string &inDestination, bool fromRef, const std::string &inName) {
        if (channelMap.contains(std::make_tuple(inSource, inDestination, fromRef))) {
            return channelMap[std::make_tuple(inSource, inDestination, fromRef)].hasData(inName);
        }
        return false;
    }

    Channel &getChannel(const std::string &inSource, const std::string &inDestination, bool fromRef) {
        return channelMap[std::make_tuple(inSource, inDestination, fromRef)];
    }

    bool setData(const std::string &inSource, const std::string &inDestination, bool fromRef, const std::string &inName, MVM::Type::Data inData) {
        if (channelMap.contains(std::make_tuple(inSource, inDestination, fromRef))) {
            return channelMap[std::make_tuple(inSource, inDestination, fromRef)].setData(inName, inData);
        }
        return false;
    }

    MVM::Type::Data getData(const std::string &inSource, const std::string &inDestination, bool fromRef, const std::string &inName) {
        if (channelMap.contains(std::make_tuple(inSource, inDestination, fromRef))) {
            return channelMap[std::make_tuple(inSource, inDestination, fromRef)].getData(inName);
        }
        return 0;
    }

};


} // namesapce MVM::Channel