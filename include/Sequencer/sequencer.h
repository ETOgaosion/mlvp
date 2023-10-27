#pragma once

#include <memory>

#include "Sequencer/testcases.h"
#include "Library/error.h"

namespace MVM {
namespace Sequencer {
class Sequencer
{
private:
    std::shared_ptr<TestCaseSet> testCases;

public:
    Sequencer() = delete;
    

    Sequencer(std::shared_ptr<TestCaseSet> inTestCases) : testCases(inTestCases) {}
    
    int getTestSize() {
        return testCases->getTestsSize();
    }

    const SerialTestsSet & getTests() {
        return testCases->getTests();
    }
};

} // namespace Sequencer

    
} // namespace MVM
