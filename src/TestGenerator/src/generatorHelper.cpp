#include "TestGenerator/generatorHelper.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Database/designPorts.h"

using namespace std;
using namespace MVM::Type;
using namespace MVM::TestGenerator;
using namespace MVM::Sequencer;
using namespace MVM::Library;
using namespace MVM::Database;

bool GeneratedUserTest::checkTestValidity(TestPoint test) {
    if (test.size() != DesignPorts::getInstance().getPortsInSize()) {
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
            std::cout << "[ERROR] TestGenerator > test size not align with port info : Port width is " << DesignPorts::getInstance().getPortsInSize() << " but test size is " << test.size() << std::endl;
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

bool GeneratedUserTest::addSerialTest(SerialTest testSet) {
    bool result = true;
    testSet.erase(std::remove_if(testSet.begin(), testSet.end(), [](TestPoint test) {return !GeneratedUserTest::checkTestValidity(test); }), testSet.end());
    userTest.push_back(testSet);
    return result;
}

bool GeneratedUserTest::addSerialTest() {
    userTest.push_back(SerialTest());
    return true;
}

bool GeneratedUserTest::addTestPoint(TestPoint test) {
    bool res = GeneratedUserTest::checkTestValidity(test);
    userTest.back().push_back(test);
    return res;
}

bool RandomGeneratorModel::addTestPoint() {
    TestPoint test;
    const auto & portsInScale = DesignPorts::getInstance().getPortsInScale();
    for (int i = 0; i < DesignPorts::getInstance().getPortsInSize(); i++) {
        uniform_int_distribution<unsigned long long> dist(0, portsInScale[i] - 1);
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

bool PortSpecGeneratorModel::checkPortSpec(PortTestSpec portTestSpec) {
    return checkPortSpec(portTestSpec.portName, portTestSpec.startIndex, portTestSpec.endIndex, portTestSpec.generatorType);
}

bool PortSpecGeneratorModel::checkPortSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType) {
    if (portTestSpecs.contains(portName) && !portTestSpecs[portName].empty() && portTestSpecs[portName].back().endIndex >= startIndex) {
        if (bugHandleDegree == Degree::HIGH) {
            throw std::runtime_error("PortSpecGeneratorModel > PortSpec is not valid");
        }
        else {
            cout << "[Error] PortSpecGeneratorModel > PortSpec is not valid" << endl;
        }
        return false;
    }
    return true;
}

bool PortSpecGeneratorModel::checkAllPortSpec() {
    if (portTestSpecs.size() != DesignPorts::getInstance().getPortsInSize()) {
        throw std::runtime_error("PortSpecGeneratorModel > missing port, PortSpec is not valid");
    }
    maxIndex = portTestSpecs.begin()->second.back().endIndex;
    return true;
}

bool PortSpecGeneratorModel::addPortTestSpec(PortTestSpec portTestSpec) {
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

bool PortSpecGeneratorModel::addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType) {
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

bool PortSpecGeneratorModel::addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, vector<unsigned long long> value) {
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

void PortSpecGeneratorModel::generateSerialTest(bool autoclear) {
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
                    throw std::runtime_error("PortSpecGeneratorModel > type not support, PortSpec is not valid");
                }
                break;
            case GeneratorType::RANDOM_GENERATOR:
            {
                uniform_int_distribution<unsigned long long> dist(0, DesignPorts::getInstance().getPortsInScale(spec.portName) - 1);
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