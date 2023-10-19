#pragma once

#include "TestsParser/testcases.h"
#include "Library/types.h"

namespace MVM {
namespace TestsParser {
class Analyser
{
private:
    TestCases testCases;
    MVM::Library::Degree checkDegree;

public:
    Analyser() = delete;
    ~Analyser() = default;

    Analyser(TestCases inTestCases);
    Analyser(TestCases inTestCases, MVM::Library::Degree inCheckDegree);

    /* process the tests: check validity, use port width to cutting input */
    bool processTests();
    
    int getTestSize() {
        return testCases.getTestsSize();
    }

    const std::vector<std::vector<unsigned long>> & getTests() {
        return testCases.getTests();
    }
};

} // namespace TestsParser

    
} // namespace MVM
