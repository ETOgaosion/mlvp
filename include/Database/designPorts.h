#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Database/designPortsGen.h"
#include "Sequencer/testcases.h"

namespace MVM {
namespace Database {
class DesignPorts {
private:
    std::unordered_map<std::string, int> portsIn;
    std::unordered_map<std::string, int> portsInIndex;
    std::vector<std::string> portsInName;
    std::vector<int> portsInScale;
    int portsInSize;
    std::unordered_map<std::string, int> portsOut;
    std::unordered_map<std::string, int> portsOutIndex;
    std::vector<std::string> portsOutName;
    std::vector<int> portsOutScale;
    int portsOutSize;

    DesignPorts() : portsIn PORT_IN_INFO, portsOut PORT_OUT_INFO {
        std::vector<std::pair<std::string, int>> portsInVec = PORT_IN_INFO;
        for (const auto & port : portsInVec) {
            portsInIndex[port.first] = portsInName.size();
            portsInName.push_back(port.first);
            portsInScale.push_back(port.second);
        }
        portsInSize = portsInScale.size();
        std::vector<std::pair<std::string, int>> portsOutVec = PORT_OUT_INFO;
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

    const std::unordered_map<std::string, int> & getPortsIn() {
        return portsIn;
    }

    int getPortInIndex(const std::string & portName) {
        return portsInIndex[portName];
    }

    int getPortsInScale(const std::string & portName) {
        return portsIn[portName];
    }

    const std::vector<std::string> & getPortsInName() {
        return portsInName;
    }

    const std::vector<int> & getPortsInScale() {
        return portsInScale;
    }

    int getPortsInScale(int index) {
        return portsInScale[index];
    }

    const std::unordered_map<std::string, int> & getPortsOut() {
        return portsOut;
    }

    int getPortOutIndex(const std::string & portName) {
        return portsOutIndex[portName];
    }

    int getPortsOutScale(const std::string & portName) {
        return portsOut[portName];
    }

    const std::vector<std::string> & getPortsOutName() {
        return portsOutName;
    }

    const std::vector<int> & getPortsOutScale() {
        return portsOutScale;
    }

    int getPortsOutScale(int index) {
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
