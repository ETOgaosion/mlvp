#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Sequencer/testcases.h"

namespace MVM {
namespace Sequencer {
class UserTest {
private:
    std::shared_ptr<TestCaseSet> userTest;

public:
    UserTest() = delete;
    ~UserTest() = default;
    UserTest(std::shared_ptr<TestCaseSet> inUserTest) : userTest(inUserTest) {}

    void generateUserTest(std::string testFilePath = "data/userTest.json");

    std::shared_ptr<TestCaseSet> getUserTest() {
        return userTest;
    }

};


} // namespace Sequencer
    
} // namespace MVM