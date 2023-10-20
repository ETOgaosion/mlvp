#pragma once

#include <vector>
#include <memory>

namespace MVM {
namespace Sequencer {
typedef std::vector<std::vector<unsigned long>> TestCase;

typedef std::vector<TestCase> TestCases;

class TestCaseSet
{
private:
    TestCases tests;
    int testsSize;

public:
    TestCaseSet() = default;
    ~TestCaseSet() = default;

    /* only use input signals */
    TestCaseSet(TestCases inTests) {
        tests = inTests;
        testsSize = tests.size();
    }

    const TestCases & getTests() {
        return tests;
    }

    int getTestsSize() {
        return testsSize;
    }

    bool setTest(int index, TestCase test) {
        if (index >= testsSize)
            return false;
        tests[index] = test;
        return true;
    }

    bool removeTest(int index) {
        if (index >= testsSize)
            return false;
        tests.erase(tests.begin() + index);
        testsSize--;
        return true;
    }

};

} // namespace Sequencer
    
} // namespace MVM
