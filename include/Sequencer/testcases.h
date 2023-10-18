#pragma once

#include <vector>

namespace MCM {
namespace Sequencer {
class TestCases
{
private:
    /* data */
    std::vector<std::vector<unsigned long>> tests;

public:
    TestCases(/* args */) = default;
    ~TestCases() = default;

    TestCases(std::vector<std::vector<unsigned long>> inTests) {
        tests = inTests;
    }
    
    void addTest(std::vector<unsigned long> test) {
        tests.push_back(test);
    }

    const std::vector<std::vector<unsigned long>> & getTests() {
        return tests;
    }
};

} // namespace Sequencer
    
} // namespace MCM
