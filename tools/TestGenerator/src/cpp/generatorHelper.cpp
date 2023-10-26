#include "generatorHelper.h"

#include <iostream>

using namespace std;
using namespace MVM::TestGenerator;
using namespace MVM::Library;
using namespace MVM::Database;

bool generateTestSetPrepare();
void generateTestSetFinish();

void addTestSet(std::vector<std::vector<int>> testSet);
void addTestSet();
void addTest(std::vector<int> test);


bool GeneratorMiddleContents::checkTestValidity(std::vector<int> test) {
    if (test.size() != MVM::Database::DesignPorts::getInstance().getPortsInSize()) {
        switch (checkDegree)
        {
        case MVM::Library::Degree::SKIP:
            return false;
            break;
        case MVM::Library::Degree::HIGH:
            throw std::runtime_error("Test size is not equal to portsInSize");
            return false;
            break;
        case MVM::Library::Degree::LOW:
        case MVM::Library::Degree::MEDIUM:
        default:
            std::cout << "[ERROR] TestGenerator > test size not align with port info : Port width is " << MVM::Database::DesignPorts::getInstance().getPortsInSize() << " but test size is " << test.size() << std::endl;
            for (auto test_i : test) {
                std::cout << test_i << " ";
            }
            std::cout << endl;
            return false;
            break;
        }
    }
    return true;
}

bool GeneratorMiddleContents::addTestSet(std::vector<std::vector<int>> testSet) {
    bool result = true;
    for (auto test : testSet) {
        result &= GeneratorMiddleContents::checkTestValidity(test);
        userTestSet.push_back(test);
    }
    return result;
}

bool GeneratorMiddleContents::addTestSet() {
    userTestSet.push_back(std::vector<std::vector<int>>());
    return true;
}

bool GeneratorMiddleContents::addTest(std::vector<int> test) {
    bool res = GeneratorMiddleContents::checkTestValidity(test);
    userTestSet.back().push_back(test);
    return res;
}

bool RandomGeneratorModel::addTest() {}

bool RandomGeneratorModel::addTestSet() {}

bool RandomGeneratorModel::addTestSets(int testSetNum) {}


bool PortSpecGeneratoorModel::addPortTestSpec(PortTestSpec portTestSpec) {
    portTestSpecs.push_back(portTestSpec);
}

bool PortSpecGeneratoorModel::addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType) {
    portTestSpecs.emplace_back(portName, startIndex, endIndex, generatorType);
}

bool PortSpecGeneratoorModel::addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, std::vector<unsigned long long> value) {
    portTestSpecs.emplace_back(portName, startIndex, endIndex, generatorType);
    portTestSpecs.back().value = value;
}

bool PortSpecGeneratoorModel::generateTest() {}