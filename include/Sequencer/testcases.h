#pragma once

#include <vector>
#include <memory>

namespace MVM {
namespace Sequencer {
typedef std::vector<std::vector<unsigned long>> SerialTest;

typedef std::vector<SerialTest> SerialTestsSet;

class TestCaseSet
{
private:
    SerialTestsSet tests;
    int testsSize;

public:
    TestCaseSet() = default;
    

    /* only use input signals */
    TestCaseSet(SerialTestsSet inTests) {
        tests = inTests;
        testsSize = tests.size();
    }

    const SerialTestsSet & getTests() {
        return tests;
    }

    int getTestsSize() {
        return testsSize;
    }

    bool setTest(int index, SerialTest test) {
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
