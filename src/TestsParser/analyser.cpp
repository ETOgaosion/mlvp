#include "TestsParser/analyser.h"
#include "Database/designPorts.h"

using namespace std;
using namespace MVM::TestsParser;
using namespace MVM::Library;

Analyser::Analyser(TestCases inTestCases) {
    testCases = inTestCases;
    checkDegree = Degree::SKIP;
    processTests();
}

Analyser::Analyser(TestCases inTestCases, Degree inCheckDegree) {
    testCases = inTestCases;
    checkDegree = inCheckDegree;
    bool res = processTests();
    if (!res && (checkDegree == Degree::LOW || checkDegree == Degree::MEDIUM)) {
        printf("Warning: Test size does not match with design ports size\n");
    }
}

bool Analyser::processTests() {
    vector<vector<unsigned long>> tests = testCases.getTests();
    vector<int> errorIndex;
    auto portsIn = Database::DesignPorts::getInstance().getPortsIn();
    auto portsInLen = Database::DesignPorts::getInstance().getPortsInLen();
    for (auto it = tests.begin(); it != tests.end(); it++) {
        /* check validity */
        if (it->size() != portsIn.size()) {
            if (checkDegree == Degree::SKIP) {
                errorIndex.push_back(it - tests.begin() - errorIndex.size());
            }
            else if (checkDegree == Degree::HIGH)
                throw "[ERROR] Analyser > Test size does not match with design ports size";
            else
                return false;
        }
        else {
            /* use port width to cut tests */
            for (auto in_it = (*it).begin(); in_it != (*it).end(); in_it++) {
                *in_it = *in_it & ((1 << portsInLen[in_it - (*it).begin()]) - 1);
            }
            testCases.setTest(it - tests.begin() - errorIndex.size(), *it);
        }
    }
    for (auto index : errorIndex) {
        testCases.removeTest(index);
    }
    return true;
}

