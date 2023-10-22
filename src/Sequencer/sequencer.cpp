#include "Sequencer/sequencer.h"
#include "Database/designPorts.h"

using namespace std;
using namespace MVM::Sequencer;
using namespace MVM::Library;

Sequencer::Sequencer(std::shared_ptr<TestCaseSet> inTestCases) {
    testCases = inTestCases;
    checkDegree = Degree::SKIP;
    processTests();
}

Sequencer::Sequencer(std::shared_ptr<TestCaseSet> inTestCases, Degree inCheckDegree) {
    testCases = inTestCases;
    checkDegree = inCheckDegree;
    bool res = processTests();
    if (!res && (checkDegree == Degree::LOW || checkDegree == Degree::MEDIUM)) {
        printf("Warning: Test size does not match with design ports size\n");
    }
}

bool Sequencer::processTests() {
    SerialTestsSet tests = testCases->getTests();
    vector<int> errorIndex;
    auto portsIn = Database::DesignPorts::getInstance().getPortsIn();
    auto portsInLen = Database::DesignPorts::getInstance().getPortsInLen();
    for (auto it = tests.begin(); it != tests.end(); it++) {
        /* check validity */
        if ((*it)[0].size() != portsIn.size()) {
            if (checkDegree == Degree::SKIP) {
                errorIndex.push_back(it - tests.begin() - errorIndex.size());
            }
            else if (checkDegree == Degree::HIGH)
                throw std::runtime_error("[ERROR] Sequencer > Test size does not match with design ports size");
            else
                return false;
        }
    }
    for (auto index : errorIndex) {
        testCases->removeTest(index);
    }
    return true;
}

