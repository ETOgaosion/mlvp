#pragma once

#include <vector>
#include <memory>

namespace MVM {
namespace TestsParser {
class TestCases
{
private:
    std::vector<std::vector<unsigned long>> tests;
    int testsSize;

public:
    TestCases() = default;
    ~TestCases() = default;

    /* only use input signals */
    TestCases(std::vector<std::vector<unsigned long>> inTests) {
        tests = inTests;
        testsSize = tests.size();
    }

    const std::vector<std::vector<unsigned long>> & getTests() {
        return tests;
    }

    int getTestsSize() {
        return testsSize;
    }

    bool setTest(int index, std::vector<unsigned long> test) {
        if (index >= testsSize)
            return false;
        tests[index] = test;
        return true;
    }

    bool setTest(int indexFirst, int indexSecont, unsigned long test) {
        if (indexFirst >= testsSize || indexSecont >= tests[indexFirst].size())
            return false;
        tests[indexFirst][indexSecont] = test;
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
