#pragma once

#include <memory>

#include "Sequencer/testcases.h"
#include "Library/types.h"

namespace MVM {
namespace Sequencer {
class Sequencer
{
private:
    std::shared_ptr<TestCaseSet> testCases;
    MVM::Library::Degree checkDegree;

public:
    Sequencer() = delete;
    ~Sequencer() = default;

    Sequencer(std::shared_ptr<TestCaseSet> inTestCases);
    Sequencer(std::shared_ptr<TestCaseSet> inTestCases, MVM::Library::Degree inCheckDegree);

    /* process the tests: check validity, use port width to cutting input */
    bool processTests();
    
    int getTestSize() {
        return testCases->getTestsSize();
    }

    const TestCases & getTests() {
        return testCases->getTests();
    }
};

} // namespace Sequencer

    
} // namespace MVM
