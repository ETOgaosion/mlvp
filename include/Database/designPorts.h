#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Database/designPortsGen.h"

namespace MVM {
namespace Database {
class DesignPorts {
private:
    std::unordered_map<std::string, MVM::Type::uint64> portsIn;
    std::unordered_map<std::string, int> portsInIndex;
    std::vector<std::string> portsInName;
    std::vector<MVM::Type::uint64> portsInScale;
    int portsInSize;
    std::unordered_map<std::string, MVM::Type::uint64> portsOut;
    std::unordered_map<std::string, int> portsOutIndex;
    std::vector<std::string> portsOutName;
    std::vector<MVM::Type::uint64> portsOutScale;
    int portsOutSize;

    DesignPorts() : portsIn PORT_IN_INFO, portsOut PORT_OUT_INFO {
        std::vector<std::pair<std::string, MVM::Type::uint64>> portsInVec = PORT_IN_INFO;
        for (const auto & port : portsInVec) {
            portsInIndex[port.first] = portsInName.size();
            portsInName.push_back(port.first);
            portsInScale.push_back(port.second);
        }
        portsInSize = portsInScale.size();
        std::vector<std::pair<std::string, MVM::Type::uint64>> portsOutVec = PORT_OUT_INFO;
        for (const auto & port : portsOutVec) {
            portsOutIndex[port.first] = portsOutName.size();
            portsOutName.push_back(port.first);
            portsOutScale.push_back(port.second);
        }
        portsOutSize = portsOutScale.size();
    }
public:
    DesignPorts(DesignPorts const &) = delete;
    void operator=(DesignPorts const &) = delete;

    static DesignPorts & getInstance() {
        static DesignPorts instance;
        return instance;
    }

    const std::unordered_map<std::string, MVM::Type::uint64> & getPortsIn() {
        return portsIn;
    }

    int getPortInIndex(const std::string & portName) {
        return portsInIndex[portName];
    }

    MVM::Type::uint64 getPortsInScale(const std::string & portName) {
        return portsIn[portName];
    }

    const std::vector<std::string> & getPortsInName() {
        return portsInName;
    }

    const std::vector<MVM::Type::uint64> & getPortsInScale() {
        return portsInScale;
    }

    MVM::Type::uint64 getPortsInScale(int index) {
        return portsInScale[index];
    }

    const std::unordered_map<std::string, MVM::Type::uint64> & getPortsOut() {
        return portsOut;
    }

    int getPortOutIndex(const std::string & portName) {
        return portsOutIndex[portName];
    }

    MVM::Type::uint64 getPortsOutScale(const std::string & portName) {
        return portsOut[portName];
    }

    const std::vector<std::string> & getPortsOutName() {
        return portsOutName;
    }

    const std::vector<MVM::Type::uint64> & getPortsOutScale() {
        return portsOutScale;
    }

    MVM::Type::uint64 getPortsOutScale(int index) {
        return portsOutScale[index];
    }

    int getPortsInSize() {
        return portsInSize;
    }

    int getPortsOutSize() {
        return portsOutSize;
    }
    
};

} // namespace Database
    
} // namespace MVM
