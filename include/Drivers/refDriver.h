/**
 * @file refDriver.h
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
#include <utility>
#include <utility>
#include <vector>
#include <memory>
#include <future>

#include "Drivers/driverModel.h"
#include "Drivers/transDriverModel.h"
#include "Transaction/transaction.h"
#include "Drivers/dutDriver.h"
#include "Drivers/simulatorDriver.h"

namespace MLVP::Driver {
class RefUnitDriver : public MLVP::Driver::DriverModel {
public:
    RefUnitDriver() = delete;
    ~RefUnitDriver() override = default;
    explicit RefUnitDriver(int inResetCycles, std::string inUnitName) : DriverModel(inResetCycles, true, std::move(inUnitName)) {}
    
    /**
     * bool drivingStep(bool isLast) override
     * @brief Driving step for simulator, <b>Notice that ref not have to drive by cycle, you can use simple transaction Handler</b>
     * @details just execute one test
     *          no implementation will also be a pure function
     *          [announcer]|[verifier] must implement
     * 
     */
};

class RefTransDriver : public TransDriver {
public:
    RefTransDriver() = delete;
    ~RefTransDriver() override = default;
    RefTransDriver(int inResetCycles, std::shared_ptr<DriverModel> in, std::unordered_map<std::string, std::shared_ptr<DriverModel>> inSimulatorDrivers) : TransDriver(inResetCycles, true, std::move(in), std::move(inSimulatorDrivers)) {}
};


} // namespace MLVP
