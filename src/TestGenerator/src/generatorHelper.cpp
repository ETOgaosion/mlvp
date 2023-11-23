/**
 * @file generatorHelper.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "TestGenerator/generatorHelper.h"

#include <iostream>
#include <utility>

#include "Database/database.h"

using namespace std;
using namespace MLVP::Type;
using namespace MLVP::TestGenerator;
using namespace MLVP::Library;
using namespace MLVP::Database;

void GeneratedUserTest::addSerialTest(const SerialTest &testSet) {
    userTest->push_back(testSet);
}

void GeneratedUserTest::addSerialTest() {
    userTest->push_back(SerialTest());
}

void GeneratedUserTest::addPortTest(const string &portName, SerialData test) {
    userTest->back()[portName] = std::move(test);
}

bool PortSpecGeneratorModel::setSize(int inSize) {
    if (size) {
        if (MLVP::Library::bugHandleDegree == MLVP::Library::Degree::HIGH)
            throw runtime_error("PortSpecGeneratorModel > Size is already set");
        else {
            return false;
        }
    }
    else {
        if (inSize <= 0) {
            if (MLVP::Library::bugHandleDegree == MLVP::Library::Degree::HIGH)
                throw runtime_error("PortSpecGeneratorModel > Size is not valid");
            else {
                return false;
            }
        }
        size = inSize;
        return true;
    }
}

bool PortSpecGeneratorModel::addPortTestSpecDefault(const string &inPortName, GeneratorType inGeneratorType, Data inValue) {
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
    return true;
}

bool PortSpecGeneratorModel::addPortTestSpecDefault(const string &inPortName, GeneratorType inGeneratorType, Data inValue, function<Data(Data)> inPostHandler) {
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
        portTestSpecDefaults[inPortName] = PortTestSpecDefault(inPortName, inGeneratorType, inValue, std::move(inPostHandler));
    }
    return true;
}

bool PortSpecGeneratorModel::checkPortSpec(PortTestSpec &portTestSpec) {
    return checkPortSpec(portTestSpec.portName, portTestSpec.startIndex, portTestSpec.endIndex, portTestSpec.generatorType);
}

bool PortSpecGeneratorModel::checkPortSpec(const string &portName, int startIndex, int &endIndex, GeneratorType generatorType) {
    if (portTestSpecs[portName].back().endIndex > size - 1) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("[PortSpecGeneratorModel] > PortSpec is not valid");
        }
        else {
            cout << "[Error] PortSpecGeneratorModel > PortSpec is not valid" << endl;
        }
        return false;
    }
    else if (portTestSpecs[portName].back().endIndex < 0) {
        if (portTestSpecs[portName].back().endIndex < -size) {
            if (bugHandleDegree == Degree::HIGH) {
                throw runtime_error("[PortSpecGeneratorModel] > PortSpec is not valid");
            }
            else {
                cout << "[Error] PortSpecGeneratorModel > PortSpec is not valid" << endl;
            }
            return false;
        }
        endIndex = size + endIndex;
    }
    if (portTestSpecs.contains(portName) && !portTestSpecs[portName].empty() &&portTestSpecs[portName].back().endIndex >= startIndex) {
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
    for (auto &portSpec : portTestSpecs) {
        if (portSpec.second.back().endIndex > size - 1) {
            throw runtime_error("PortSpecGeneratorModel > PortSpec is not valid");
            return false;
        }
    }
    return true;
}

bool PortSpecGeneratorModel::addPortTestSpec(string portName, int startIndex, int endIndex, GeneratorType generatorType, const SerialData &value) {
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].back().value = value;
    }
    else {
        portTestSpecs.emplace(portName, vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType)));
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
    for (auto &portSpec : portTestSpecDefaults) {
        switch (portSpec.second.generatorType)
        {
        case GeneratorType::DIRECT_INPUT:
            for (int i = 0; i < size; i++) {
                SerialTest[portSpec.second.portName][i] = portSpec.second.postHandler(portSpec.second.value);
            }
            break;
        case GeneratorType::RANDOM_GENERATOR:
        {
            uniform_int_distribution<Data> dist(0, portSpec.second.value);
            for (int i = 0; i < size; i++) {
                SerialTest[portSpec.second.portName][i] = portSpec.second.postHandler(dist(rng));
            }
        }
            break;
        }
    }
    for (auto &portSpec : portTestSpecs) {
        for (auto &spec : portSpec.second) {
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
                uniform_int_distribution<Data> dist(0, spec.value[0]);
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