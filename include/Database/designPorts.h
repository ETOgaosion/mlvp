#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Database/designPortsGen.h"

namespace MVM {
namespace Database {
class DesignPorts {
private:
    std::unordered_map<std::string, unsigned long long> portsIn;
    std::unordered_map<std::string, int> portsInIndex;
    std::vector<std::string> portsInName;
    std::vector<unsigned long long> portsInScale;
    int portsInSize;
    std::unordered_map<std::string, unsigned long long> portsOut;
    std::unordered_map<std::string, int> portsOutIndex;
    std::vector<std::string> portsOutName;
    std::vector<unsigned long long> portsOutScale;
    int portsOutSize;

    DesignPorts() : portsIn PORT_IN_INFO, portsOut PORT_OUT_INFO {
        std::vector<std::pair<std::string, unsigned long long>> portsInVec = PORT_IN_INFO;
        for (const auto & port : portsInVec) {
            portsInIndex[port.first] = portsInName.size();
            portsInName.push_back(port.first);
            portsInScale.push_back(port.second);
        }
        portsInSize = portsInScale.size();
        std::vector<std::pair<std::string, unsigned long long>> portsOutVec = PORT_OUT_INFO;
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

    const std::unordered_map<std::string, unsigned long long> & getPortsIn() {
        return portsIn;
    }

    int getPortInIndex(const std::string & portName) {
        return portsInIndex[portName];
    }

    unsigned long long getPortsInScale(const std::string & portName) {
        return portsIn[portName];
    }

    const std::vector<std::string> & getPortsInName() {
        return portsInName;
    }

    const std::vector<unsigned long long> & getPortsInScale() {
        return portsInScale;
    }

    unsigned long long getPortsInScale(int index) {
        return portsInScale[index];
    }

    const std::unordered_map<std::string, unsigned long long> & getPortsOut() {
        return portsOut;
    }

    int getPortOutIndex(const std::string & portName) {
        return portsOutIndex[portName];
    }

    unsigned long long getPortsOutScale(const std::string & portName) {
        return portsOut[portName];
    }

    const std::vector<std::string> & getPortsOutName() {
        return portsOutName;
    }

    const std::vector<unsigned long long> & getPortsOutScale() {
        return portsOutScale;
    }

    unsigned long long getPortsOutScale(int index) {
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
