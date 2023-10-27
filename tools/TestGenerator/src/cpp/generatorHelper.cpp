#include "generatorHelper.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Database/designPorts.h"

using namespace std;
using namespace MVM::TestGenerator;
using namespace MVM::Sequencer;
using namespace MVM::Library;
using namespace MVM::Database;

bool GeneratorMiddleContents::checkTestValidity(TestPoint test) {
    if (test.size() != MVM::Database::DesignPorts::getInstance().getPortsInSize()) {
        switch (bugHandleDegree)
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

bool GeneratorMiddleContents::generateTestSetPrepare() {
    if (std::filesystem::exists(testSetFilePath)) {
        std::filesystem::rename(testSetFilePath, testSetFilePath + ".bak");
    }
    ofstream testSetFile(testSetFilePath, ofstream::trunc);
    if (!testSetFile.is_open()) {
        throw std::runtime_error("TestGenerator > Cannot open testSetFile");
        return false;
    }
    testSetFile << "[" << endl;
    return true;
}

void GeneratorMiddleContents::generateMiddleContents() {
    ofstream testSetFile(testSetFilePath, ofstream::app);
    if (!testSetFile.is_open()) {
        throw std::runtime_error("TestGenerator > Cannot open testSetFile");
    }
    for (auto testSet_it = userTest.begin(); testSet_it != userTest.end(); testSet_it++) {
        testSetFile << "[";
        for (auto testPoint_it = testSet_it->begin(); testPoint_it != testSet_it->end(); testPoint_it++) {
            testSetFile << "[";
            for (auto test_it = testPoint_it->begin(); test_it != testPoint_it->end(); test_it++) {
                testSetFile << *test_it;
                if (test_it != testPoint_it->end() - 1) {
                    testSetFile << ",";
                }
            }
            testSetFile << "]";
            if (testPoint_it != testSet_it->end() - 1) {
                testSetFile << ",";
            }
        }
        testSetFile << "]";
        if (testSet_it != userTest.end() - 1) {
            testSetFile << ",";
        }
        testSetFile << endl;
    }
}

void GeneratorMiddleContents::generateTestSetFinish() {
    ofstream testSetFile(testSetFilePath, ofstream::app);
    if (!testSetFile.is_open()) {
        throw std::runtime_error("TestGenerator > Cannot open testSetFile");
    }
    testSetFile << "]";
}

bool GeneratorMiddleContents::addSerialTest(SerialTest testSet) {
    bool result = true;
    testSet.erase(std::remove_if(testSet.begin(), testSet.end(), [](TestPoint test) {return !GeneratorMiddleContents::checkTestValidity(test); }), testSet.end());
    userTest.push_back(testSet);
    return result;
}

bool GeneratorMiddleContents::addSerialTest() {
    userTest.push_back(SerialTest());
    return true;
}

bool GeneratorMiddleContents::addTestPoint(TestPoint test) {
    bool res = GeneratorMiddleContents::checkTestValidity(test);
    userTest.back().push_back(test);
    return res;
}

bool RandomGeneratorModel::addTestPoint() {
    TestPoint test;
    const auto & portsInLen = MVM::Database::DesignPorts::getInstance().getPortsInLen();
    for (int i = 0; i < MVM::Database::DesignPorts::getInstance().getPortsInSize(); i++) {
        uniform_int_distribution<unsigned long long> dist(0, portsInLen[i] - 1);
        test.push_back(dist(rng));
    }
    return middleContents->addTestPoint(test);
}

bool RandomGeneratorModel::addSerialTest(int testNum) {
    bool res = true;
    middleContents->addSerialTest();
    for (int i = 0; i < testNum; i++) {
        res &= addTestPoint();
    }
    return true;
}

bool RandomGeneratorModel::addSerialTestsSet(std::vector<int> serialTestSize) {
    bool res = true;
    for (auto size : serialTestSize) {
        res &= addSerialTest(size);
    }
    return true;
}

bool PortSpecGeneratoorModel::checkPortSpec(PortTestSpec portTestSpec) {
    return checkPortSpec(portTestSpec.portName, portTestSpec.startIndex, portTestSpec.endIndex, portTestSpec.generatorType);
}

bool PortSpecGeneratoorModel::checkPortSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType) {
    if (portTestSpecs.contains(portName) && !portTestSpecs[portName].empty() && portTestSpecs[portName].back().endIndex >= startIndex) {
        if (bugHandleDegree == Degree::HIGH) {
            throw std::runtime_error("PortSpecGeneratoorModel > PortSpec is not valid");
        }
        else {
            cout << "[Error] PortSpecGeneratoorModel > PortSpec is not valid" << endl;
        }
        return false;
    }
    return true;
}

bool PortSpecGeneratoorModel::checkAllPortSpec() {
    if (portTestSpecs.size() != DesignPorts::getInstance().getPortsInSize()) {
        throw std::runtime_error("PortSpecGeneratoorModel > missing port, PortSpec is not valid");
    }
    int prevIndex = portTestSpecs.begin()->second.back().endIndex;
    for (auto portSpec : portTestSpecs) {
        maxIndex = portSpec.second.back().endIndex;
        if (maxIndex != prevIndex) {
            throw std::runtime_error("PortSpecGeneratoorModel > serial length not consistent, PortSpec is not valid");
        }
        prevIndex = maxIndex;
        for (int i = 0; i < portSpec.second.size() - 1; i++) {
            if (portSpec.second[i].endIndex != portSpec.second[i + 1].startIndex - 1) {
                throw std::runtime_error("PortSpecGeneratoorModel > not continuous portspec, PortSpec is not valid");
                return false;
            }
        }
    }
    return true;
}

bool PortSpecGeneratoorModel::addPortTestSpec(PortTestSpec portTestSpec) {
    bool res = true;
    if (portTestSpecs.contains(portTestSpec.portName)) {
        res &= checkPortSpec(portTestSpec);
        portTestSpecs[portTestSpec.portName].push_back(portTestSpec);
    }
    else {
        portTestSpecs.emplace(std::make_pair(portTestSpec.portName, std::vector<PortTestSpec>(1, portTestSpec)));
    }
    return res;
}

bool PortSpecGeneratoorModel::addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType) {
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType);
    }
    else {
        portTestSpecs.emplace(std::make_pair(portName, std::vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType))));
    }
    return res;
}

bool PortSpecGeneratoorModel::addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, TestPoint value) {
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].back().value = value;
    }
    else {
        portTestSpecs.emplace(std::make_pair(portName, std::vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType))));
        portTestSpecs[portName].back().value = value;
    }
    return res;
}

void PortSpecGeneratoorModel::generateSerialTest(bool autoclear) {
    checkAllPortSpec();
    SerialTest serialTest(maxIndex + 1, TestPoint(DesignPorts::getInstance().getPortsInSize(), 0));
    for (auto & portSpec : portTestSpecs) {
        for (auto & spec : portSpec.second) {
            switch (spec.generatorType)
            {
            case GeneratorType::BASE:
            case GeneratorType::DIRECT_INPUT:
                if (spec.value.size() == 1) {
                    for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                        serialTest[i][DesignPorts::getInstance().getPortInIndex(spec.portName)] = spec.value[0];
                    }
                }
                else if (spec.value.size() == spec.endIndex - spec.startIndex + 1) {
                    for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                        serialTest[i][DesignPorts::getInstance().getPortInIndex(spec.portName)] = spec.value[i - spec.startIndex];
                    }
                }
                else {
                    throw std::runtime_error("PortSpecGeneratoorModel > type not support, PortSpec is not valid");
                }
                break;
            case GeneratorType::RANDOM_GENERATOR:
            {
                uniform_int_distribution<unsigned long long> dist(0, DesignPorts::getInstance().getPortsInLen(spec.portName) - 1);
                for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                    serialTest[i][DesignPorts::getInstance().getPortInIndex(spec.portName)] = dist(rng);
                }
            }
                break;
            }
        }
    }
    middleContents->addSerialTest(serialTest);
    if (autoclear) {
        clearSerialTest();
    }
}