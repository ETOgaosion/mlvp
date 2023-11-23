/**
 * @file simulator.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "Library/types.h"

namespace MLVP::Simulator {
class Simulator
{
private:
    bool connectToRef;

public:
    Simulator() = delete;
    virtual ~Simulator() = default;

    explicit Simulator(bool inConnectToRef) : connectToRef(inConnectToRef) {}

    /**
     * @brief Assign Input to Simulator
     * @details split input and execution, because in some cases response longer than 1 cycle
     * 
     * @param MLVP::Type::PortsData inSignal 
     * @return true 
     * @return false 
     */
    virtual bool assignInput(MLVP::Type::PortsData inSignal) = 0;

    /**
     * @brief Use return value to check if transaction is done
     * 
     * @return true 
     * @return false 
     */
    virtual bool exec() = 0;

    /**
     * @brief Get the Output ports-data
     * 
     * @return const MLVP::Type::PortsData & 
     */
    virtual const MLVP::Type::PortsData &getOutput() = 0;
};

}