/**
 * @file error.h
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
#include <iostream>

namespace MLVP::Library {
enum class Degree {
    SKIP = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
};

static Degree bugHandleDegree = BUGDEGREE;

static void FunctionNotImplementError(const std::string &functionName) {
    switch (bugHandleDegree)
    {
    case MLVP::Library::Degree::HIGH:
        throw std::runtime_error("use base class not implemented methed");
        break;
    
    default:
        std::cout << "[ERROR] " << functionName << " > use base class not implemented methed" << std::endl;
        break;
    }
}

} // namespace MLVP::Library