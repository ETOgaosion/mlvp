#pragma once

#include <memory>

#include "Library/error.h"
#include "Library/types.h"

namespace MVM {
namespace Sequencer {
class Sequencer
{
private:
    std::shared_ptr<MVM::Type::SerialTestsSet> testCases;

public:
    Sequencer() = delete;

    Sequencer(std::shared_ptr<MVM::Type::SerialTestsSet> inTestCases) : testCases(inTestCases) {}
    
    int getTestSize() {
        return testCases->size();
    }

    const MVM::Type::SerialTestsSet & getTests() {
        return *testCases;
    }
};

} // namespace Sequencer

    
} // namespace MVM
