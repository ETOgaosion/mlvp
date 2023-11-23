/**
 * @file dutDriver.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <verilated.h>
#include "verilated_vcd_c.h"

#include "Drivers/driverModel.h"
#include "Drivers/transDriverModel.h"
#include "Transaction/transaction.h"
#include "Drivers/simulatorDriver.h"
#include "Channel/channel.h"

namespace MLVP::Driver {
class DutUnitDriver : public MLVP::Driver::DriverModel {
protected:
    const std::unique_ptr<VerilatedContext> contextp;
    const std::unique_ptr<VerilatedVcdC> tfp;
    std::string logPath;
    
public:
    DutUnitDriver() = delete;
    ~DutUnitDriver() override = default;

    DutUnitDriver(std::string inUnitName, int inDriverID, const std::string &inLogPath) : DriverModel(std::move(inUnitName)), logPath(inLogPath + "/Driver" + std::to_string(inDriverID)), contextp(std::make_unique<VerilatedContext>()), tfp(std::make_unique<VerilatedVcdC>()) {
        std::filesystem::create_directories(logPath);
        Verilated::traceEverOn(true);
        contextp->traceEverOn(true);
        channels.clear();
    }

    /**
     * bool drivingStep(bool isLast) override
     * @brief Driving step for simulator
     * @details just execute one test
     *          no implementation will also be a pure function
     *          [announcer]|[verifier] must implement
     * 
     */
};

class DutTransDriver : public TransDriver {
public:
    DutTransDriver() = delete;
    ~DutTransDriver() override = default;
    DutTransDriver(std::unique_ptr<DriverModel> in, int inSimulatorSetIndex, std::vector<std::string> inSimulatorNames) : TransDriver(std::move(in), inSimulatorSetIndex, std::move(inSimulatorNames)) {}

};

} // namespace MLVP::Driver