#pragma once

#include <string>

namespace MVM {
namespace Library {
enum class Degree {
    SKIP = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
};

static Degree bugHandleDegree = BUGDEGREE;

static void FunctionNotImplementError(std::string functionName) {
    switch (bugHandleDegree)
    {
    case MVM::Library::Degree::HIGH:
        throw std::runtime_error("use base class not implemented methed");
        break;
    
    default:
        std::cout << "[ERROR] " << functionName << " > use base class not implemented methed" << std::endl;
        break;
    }
}

} // namespace Library

} // namespace MVM