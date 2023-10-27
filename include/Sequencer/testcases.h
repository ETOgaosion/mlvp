#pragma once

#include <vector>
#include <memory>

namespace MVM {
namespace Sequencer {
typedef std::vector<unsigned long long> TestPoint;

typedef std::vector<TestPoint> SerialTest;

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

    void addSerialTest(SerialTest test) {
        tests.push_back(test);
        testsSize++;
    }

    void addSerialTest() {
        tests.push_back({});
        testsSize++;
    }

    void addTestPoint(TestPoint test) {
        tests.back().push_back(test);
    }

    void addTestPoint() {
        tests.back().push_back({});
    }

    void addTest(unsigned long long test) {
        tests.back().back().push_back(test);
    }

};

} // namespace Sequencer
    
} // namespace MVM
