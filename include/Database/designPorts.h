#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Database/designPortsGen.h"

namespace MVM {
namespace Database {
class DesignPorts {
private:
    std::unordered_map<std::string, int> portsIn;
    std::vector<std::string> portsInName;
    std::vector<int> portsInLen;
    int portsInSize;
    std::unordered_map<std::string, int> portsOut;
    std::vector<std::string> portsOutName;
    std::vector<int> portsOutLen;
    int portsOutSize;

    DesignPorts() : portsIn PORT_IN_INFO, portsOut PORT_OUT_INFO {
        for (const auto & port : portsIn) {
            portsInName.push_back(port.first);
            portsInLen.push_back(port.second);
        }
        portsInSize = portsInLen.size();
        for (const auto & port : portsOut) {
            portsOutName.push_back(port.first);
            portsOutLen.push_back(port.second);
        }
        portsOutSize = portsOutLen.size();
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

    int getPortsInLen(const std::string & portName) {
        return portsIn[portName];
    }

    const std::vector<std::string> & getPortsInName() {
        return portsInName;
    }

    const std::vector<int> & getPortsInLen() {
        return portsInLen;
    }

    const std::unordered_map<std::string, int> & getPortsOut() {
        return portsOut;
    }

    int getPortsOutLen(const std::string & portName) {
        return portsOut[portName];
    }

    const std::vector<std::string> & getPortsOutName() {
        return portsOutName;
    }

    const std::vector<int> & getPortsOutLen() {
        return portsOutLen;
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
