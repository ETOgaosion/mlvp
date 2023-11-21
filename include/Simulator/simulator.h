#pragma once

#include "Library/types.h"

namespace MVM::Simulator {
class Simulator
{
private:

public:
    Simulator() = default;
    virtual ~Simulator() = default;

    /**
     * @brief Assign Input to Simulator
     * @details split input and execution, because in some cases response longer than 1 cycle
     * 
     * @param MVM::Type::SerialTestSingle inSignal 
     * @return true 
     * @return false 
     */
    virtual bool assignInput(MVM::Type::SerialTestSingle inSignal) = 0;

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
     * @return const MVM::Type::SerialTestSingle & 
     */
    virtual const MVM::Type::SerialTestSingle &getOutput() = 0;
};

}