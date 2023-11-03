#pragma once

#include <vector>
#include <memory>

#include "TestGenerator/generatorHelper.h"
#include "Library/types.h"

namespace MVM {
namespace Sequencer {
class TestCaseSet
{
private:
    std::shared_ptr<MVM::TestGenerator::GeneratedUserTest> tests;
    int testsSize;

public:
    TestCaseSet() = delete;
    

    /* only use input signals */
    TestCaseSet(std::shared_ptr<MVM::TestGenerator::GeneratedUserTest> inTests) : tests(inTests) {
        testsSize = tests->getTestSize();
    }

    const MVM::Type::SerialTestsSet & getTests() {
        return tests->getTests();
    }

    int getTestsSize() {
        return testsSize;
    }

};

} // namespace Sequencer
    
} // namespace MVM
