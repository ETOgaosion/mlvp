/**
 * @file generatorHelper.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "TestGenerator/generatorHelper.h"

#include <functional>
#include <iostream>
#include <utility>

#include "Database/database.h"
#include "Library/types.h"
#include "Library/utils.h"

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

bool PortSpecGeneratorModel::addPortTestSpecDefault(const string &inPortName, GeneratorType inGeneratorType, Data inValue) {
    if (portTestSpecDefaults.contains(inPortName)) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("PortSpecGeneratorModel > PortSpecDefault is not valid");
        }
        else if (bugHandleDegree != Degree::SKIP) {
            return false;
        }
    }
    else {
        portTestSpecDefaults.emplace(inPortName, PortTestSpecDefault(inPortName, inGeneratorType, inValue));
    }
    return true;
}

bool PortSpecGeneratorModel::addPortTestSpecDefault(const string &inPortName, GeneratorType inGeneratorType, Data inValue, optional<function<bool(Data)>> inConstrain, optional<function<Data(Data)>> inPostHandler) {
    if (portTestSpecDefaults.contains(inPortName)) {
        if (bugHandleDegree == Degree::HIGH) {
            throw runtime_error("PortSpecGeneratorModel > PortSpecDefault is not valid");
        }
        else if (bugHandleDegree != Degree::SKIP) {
            return false;
        }
    }
    else {
        portTestSpecDefaults.emplace(inPortName, PortTestSpecDefault(inPortName, inGeneratorType, inValue, std::move(inConstrain), std::move(inPostHandler)));
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

bool PortSpecGeneratorModel::addPortTestSpec(string portName, int startIndex, int endIndex, GeneratorType generatorType, const SerialData &value, bool isBitWidth) {
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType, value, isBitWidth);
        portTestSpecs[portName].back().value = value;
    }
    else {
        portTestSpecs.emplace(portName, vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType, value, isBitWidth)));
    }
    return res;
}

bool PortSpecGeneratorModel::addPortTestSpec(string portName, int startIndex, int endIndex, GeneratorType generatorType, const SerialData &value, bool isBitWidth, optional<function<bool(Data)>> inConstrain, optional<function<Data(Data)>> inPostHandler) {
    bool res = true;
    if (portTestSpecs.contains(portName)) {
        res &= checkPortSpec(portName, startIndex, endIndex, generatorType);
        portTestSpecs[portName].emplace_back(portName, startIndex, endIndex, generatorType, value, isBitWidth, inConstrain, inPostHandler);
        portTestSpecs[portName].back().value = value;
    }
    else {
        portTestSpecs.emplace(portName, vector<PortTestSpec>(1, PortTestSpec(portName, startIndex, endIndex, generatorType, value, isBitWidth, inConstrain, inPostHandler)));
    }
    return res;
}

void PortSpecGeneratorModel::generateSerialTest(bool autoclear) {
    if (!size) {
        throw runtime_error("PortSpecGeneratorModel > Size is not set");
    }
    checkAllPortSpec();
    SerialTest serialTest({});
    for (auto &portSpec : portTestSpecDefaults) {
        serialTest[portSpec.second.portName] = SerialData(size);
        switch (portSpec.second.generatorType)
        {
        case GeneratorType::DIRECT_INPUT:
            for (int i = 0; i < size; i++) {
                serialTest[portSpec.second.portName][i] = portSpec.second.postHandler(portSpec.second.value);
            }
            break;
        case GeneratorType::RANDOM_GENERATOR:
        {
            for (int i = 0; i < size; i++) {
                Data randVal = RandomGenerator::getInstance().getRandomData(portSpec.second.maxVal);
                while (!portSpec.second.constrain(randVal)) {
                    randVal = RandomGenerator::getInstance().getRandomData(portSpec.second.maxVal);
                }
                serialTest[portSpec.second.portName][i] = portSpec.second.postHandler(randVal);
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
                        serialTest[spec.portName][i] = spec.postHandler(spec.value[0]);
                    }
                }
                else if (spec.value.size() == spec.endIndex - spec.startIndex + 1) {
                    for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                        serialTest[spec.portName][i] = spec.postHandler(spec.value[i - spec.startIndex]);
                    }
                }
                else {
                    throw runtime_error("PortSpecGeneratorModel > type not support, PortSpec is not valid");
                }
                break;
            case GeneratorType::RANDOM_GENERATOR:
            {
                for (int i = spec.startIndex; i <= spec.endIndex; i++) {
                    Data randVal = RandomGenerator::getInstance().getRandomData(spec.maxVal);
                    while (!spec.constrain(randVal)) {
                        randVal = RandomGenerator::getInstance().getRandomData(spec.maxVal);
                    }
                    serialTest[spec.portName][i] = spec.postHandler(randVal);
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