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
    

    Sequencer(std::shared_ptr<TestCaseSet> inTestCases);

    /* process the tests: check validity, use port width to cutting input */
    bool processTests();
    
    int getTestSize() {
        return testCases->getTestsSize();
    }

    const SerialTestsSet & getTests() {
        return testCases->getTests();
    }
};

} // namespace Sequencer

    
} // namespace MVM
