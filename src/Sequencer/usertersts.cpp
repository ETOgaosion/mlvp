#include "Sequencer/usertests.h"

#include <iostream>
#include <fstream>

#include "json.hpp"

using namespace std;
using namespace MVM::Sequencer;
using json = nlohmann::json;

void UserTest::generateUserTest(std::string testFilePath) {
    ifstream testFile(testFilePath);
    json testJson;
    testFile >> testJson;
    testFile.close();
    for (auto testSet : testJson) {
        userTest->addSerialTest();
        for (auto testPoint : testSet) {
            userTest->addTestPoint();
            for (auto testPoint_i : testPoint) {
                userTest->addTest(testPoint_i.template get<unsigned long long>());
            }
        }
    }
}