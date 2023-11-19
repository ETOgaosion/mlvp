#include "TestGenerator/generatorHelper.h"

#include <iostream>

#include "Database/designPorts.h"

using namespace std;
using namespace MVM::Type;
using namespace MVM::TestGenerator;
using namespace MVM::Library;
using namespace MVM::Database;

void GeneratedUserTest::addSerialTest(SerialTest testSet) {
    userTest->push_back(testSet);
}

void GeneratedUserTest::addSerialTest() {
    userTest->push_back(SerialTest());
}

void GeneratedUserTest::addPortTest(string portName, TestData test) {
    userTest->back()[portName] = test;
}

bool PortSpecGeneratorModel::setSize(int inSize) {
    if (size) {
        if (MVM::Library::bugHandleDegree == MVM::Library::Degree::HIGH)
            throw std::runtime_error("PortSpecGeneratorModel > Size is already set");
        else {
            return false;
        }
    }
    else {
        if (inSize <= 0) {
            if (MVM::Library::bugHandleDegree == MVM::Library::Degree::HIGH)
                throw std::runtime_error("PortSpecGeneratorModel > Size is not valid");
            else {
                return false;
            }
        }
        size = inSize;
        return true;
    }
}

bool PortSpecGeneratorModel::addPortTestSpecDefault(string inPortName, GeneratorType inGeneratorType, uint64 inValue) {
    if (!size) {
        throw runtime_error("PortSpecGeneratorModel > Size is not set");
    }
    if (portTestSpecDefaults.contains(inPortName)) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("PortSpecGeneratorModel > PortSpecDefault is not valid");
        }
        else if (bugHandleDegree != Degree::SKIP) {
            return false;
        }
    }
    else {
        portTestSpecDefaults[inPortName] = PortTestSpecDefault(inPortName, inGeneratorType, inValue);
    }

}

bool PortSpecGeneratorModel::addPortTestSpecDefault(string inPortName, GeneratorType inGeneratorType, uint64 inValue, function<uint64(uint64)> inPostHandler) {
    if (!size) {
        throw runtime_error("PortSpecGeneratorModel > Size is not set");
    }
    if (portTestSpecDefaults.contains(inPortName)) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("PortSpecGeneratorModel > PortSpecDefault is not valid");
        }
        else if (bugHandleDegree != Degree::SKIP) {
            return false;
        }
    }
    else {
        portTestSpecDefaults[inPortName] = PortTestSpecDefault(inPortName, inGeneratorType, inValue, inPostHandler);
    }
}

bool PortSpecGeneratorModel::checkPortSpec(PortTestSpec &portTestSpec) {
    return checkPortSpec(portTestSpec.portName, portTestSpec.startIndex, portTestSpec.endIndex, portTestSpec.generatorType);
}

bool PortSpecGeneratorModel::checkPortSpec(string portName, int startIndex, int &endIndex, GeneratorType generatorType) {
    if (portTestSpecs[portName].back().endIndex > size - 1) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("[PortSpecGeneratorModel] > PortSpec is not valid");
        }
        else {
            cout << "[Error] PortSpecGeneratorModel > PortSpec is not valid" << endl;
        }
        return false;
    }
    else if (portTestSpecs[portName].back().endIndex < -size) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("[PortSpecGeneratorModel] > PortSpec is not valid");
        }
        else {
            cout << "[Error] PortSpecGeneratorModel > PortSpec is not valid" << endl;
        }
        return false;
    }
    else if (portTestSpecs[portName].back().endIndex < 0) {
        endIndex = size + endIndex;
    }
    if (portTestSpecs.contains(portName) && !portTestSpecs[portName].empty() && portTestSpecs[portName].back().endIndex >= startIndex) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("PortSpecGeneratorModel > PortSpec is not valid");
        }
        else {
            cout << "[Error] PortSpecGeneratorModel > PortSpec is not valid" << endl;
        }
        return false;
    }
    return true;
}

bool PortSpecGeneratorModel::checkAllPortSpec() {
    for (auto & portSpec : portTestSpecs) {
        if (portSpec.second.back().endIndex > size - 1) {
            throw runtime_error("PortSpecGeneratorModel > PortSpec is not valid");
            return false;
        }
    }
    return true;
}

bool PortSpecGeneratorModel::addPortTestSpec(PortTestSpec portTestSpec) {
    if (!size) {
        throw runtime_error("PortSpecGeneratorModel > Size is not set");
    }
    bool res = true;
    if (portTestSpecs.contains(portTestSpec.portName)) {
        res &= checkPortSpec(portTestSpec);
        portTestSpecs[portTestSpec.portName].push_back(portTestSpec);
    }
    else {
        portTestSpecs.emplace(make_pair(portTestSpec.portName, vector<PortTestSpec>(1, portTestSpec)));
    }
    return res;
}

bool PortSpecGeneratorModel::addPortTestSpec(string portName, int startIndex, int endIndex, GeneratorType generatorType) {
    if (!size) {
        throw runtime_error("PortSpecGeneratorModel > Size is not set");
    }
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType);
    }
    else {
        portTestSpecs.emplace(make_pair(portName, vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType))));
    }
    return res;
}

bool PortSpecGeneratorModel::addPortTestSpec(string portName, int startIndex, int endIndex, GeneratorType generatorType, TestData value) {
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].back().value = value;
    }
    else {
        portTestSpecs.emplace(make_pair(portName, vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType))));
        portTestSpecs[portName].back().value = value;
    }
    return res;
}

void PortSpecGeneratorModel::generateSerialTest(bool autoclear) {
    if (!size) {
        throw runtime_error("PortSpecGeneratorModel > Size is not set");
    }
    checkAllPortSpec();
    SerialTest SerialTest(size);
    for (auto & portSpec : portTestSpecDefaults) {
        switch (portSpec.second.generatorType)
        {
        case GeneratorType::DIRECT_INPUT:
            for (int i = 0; i < size; i++) {
                SerialTest[portSpec.second.portName][i] = portSpec.second.postHandler(portSpec.second.value);
            }
            break;
        case GeneratorType::RANDOM_GENERATOR:
        {
            uniform_int_distribution<uint64> dist(0, DesignPorts::getInstance().getPortsInScale(portSpec.second.portName) - 1);
            for (int i = 0; i < size; i++) {
                SerialTest[portSpec.second.portName][i] = portSpec.second.postHandler(dist(rng));
            }
        }
            break;
        }
    }
    for (auto & portSpec : portTestSpecs) {
        for (auto & spec : portSpec.second) {
            switch (spec.generatorType)
            {
            case GeneratorType::DIRECT_INPUT:
                if (spec.value.size() == 1) {
                    for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                        SerialTest[spec.portName][i] = spec.postHandler(spec.value[0]);
                    }
                }
                else if (spec.value.size() == spec.endIndex - spec.startIndex + 1) {
                    for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                        SerialTest[spec.portName][i] = spec.postHandler(spec.value[i - spec.startIndex]);
                    }
                }
                else {
                    throw runtime_error("PortSpecGeneratorModel > type not support, PortSpec is not valid");
                }
                break;
            case GeneratorType::RANDOM_GENERATOR:
            {
                uniform_int_distribution<uint64> dist(0, DesignPorts::getInstance().getPortsInScale(spec.portName) - 1);
                for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                    SerialTest[spec.portName][i] = spec.postHandler(dist(rng));
                }
            }
                break;
            }
        }
    }
    middleContents->addSerialTest(SerialTest);
    if (autoclear) {
        clearSerialTest();
    }
}