/**
 * @file generatorHelper.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <cmath>

#include "Library/error.h"
#include "Library/types.h"

namespace MLVP::TestGenerator {

class GeneratedUserTest {
private:
    std::shared_ptr<MLVP::Type::SerialTestSet> userTest;

public:
    GeneratedUserTest() : userTest(std::make_shared<MLVP::Type::SerialTestSet>()) {}
    ~GeneratedUserTest() = default;

    void addSerialTest(const MLVP::Type::SerialTest &testSet);
    void addSerialTest();
    void addPortTest(const std::string &portName, MLVP::Type::SerialData test);

    std::shared_ptr<MLVP::Type::SerialTestSet> &getTests() {
        return userTest;
    }

    int getTestSize() { return (int)userTest->size(); }
};

enum class GeneratorType {
    DIRECT_INPUT = 0,
    RANDOM_GENERATOR = 1
};

struct PortTestSpecDefault {
    std::string portName;
    MLVP::Type::Data maxVal;
    GeneratorType generatorType;
    MLVP::Type::Data value;
    std::function<bool(MLVP::Type::Data)> constrain;
    std::function<MLVP::Type::Data(MLVP::Type::Data)> postHandler;

    PortTestSpecDefault() = delete;
    /**
     * @brief Construct a new Port Test Spec Default object
     * 
     * @param inPortName 
     * @param inGeneratorType 
     * @param inValue if generator type is DIRECT_INPUT, this value will be used as default value, else it will be seen as the <b>width</b> of this port, convert from verilog [max : 0] to max + 1
     */
    PortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue) : portName(std::move(inPortName)), generatorType(inGeneratorType), value(inValue) {
        if (generatorType == GeneratorType::RANDOM_GENERATOR) {
            maxVal = (MLVP::Type::Data)std::pow(2, inValue) - 1;
        }
        constrain = [](MLVP::Type::Data in) { return true; };
        postHandler = [](MLVP::Type::Data in) { return in; };
    }
    /**
     * @brief Construct a new Port Test Spec Default object
     * 
     * @param inPortName 
     * @param inGeneratorType 
     * @param inValue if generator type is DIRECT_INPUT, this value will be used as default value, else it will be seen as the <b>bit width</b> of this port, convert from verilog [max : 0] to max + 1
     * @param inConstrain if value not satisfy, generate again
     * @param inPostHandler post handle the value
     */
    PortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue, std::optional<std::function<bool(MLVP::Type::Data)>> inConstrain, std::optional<std::function<MLVP::Type::Data(MLVP::Type::Data)>> inPostHandler) : portName(std::move(inPortName)), generatorType(inGeneratorType), value(inValue) {
        if (generatorType == GeneratorType::RANDOM_GENERATOR) {
            maxVal = (MLVP::Type::Data)std::pow(2, inValue) - 1;
        }
        if (inConstrain.has_value()) {
            constrain = inConstrain.value();
        }
        else {
            constrain = [](MLVP::Type::Data in) { return true; };
        }
        if (inPostHandler.has_value()) {
            postHandler = inPostHandler.value();
        }
        else {
            postHandler = [](MLVP::Type::Data in) { return in; };
        }
    }
};

struct PortTestSpec {
    std::string portName;
    GeneratorType generatorType;
    int startIndex;
    int endIndex;
    MLVP::Type::Data maxVal;
    MLVP::Type::SerialData value;
    std::function<bool(MLVP::Type::Data)> constrain;
    std::function<MLVP::Type::Data(MLVP::Type::Data)> postHandler;

    PortTestSpec() = delete;
    /**
     * @brief Construct a new Port Test Spec object
     * 
     * @param inPortName 
     * @param inStartIndex 
     * @param inEndIndex 
     * @param inGeneratorType 
     * @param inValue if generator type is DIRECT_INPUT, this value will be used as default value, else it will be seen as the <b>width</b> of this port, convert with verilog [max : 0]'s max + 1
     */
    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType, MLVP::Type::SerialData inValue, bool isBitWidth) : portName(std::move(inPortName)), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType), value(std::move(inValue)) {
        if (generatorType == GeneratorType::RANDOM_GENERATOR) {
            if (isBitWidth) {
                maxVal = (MLVP::Type::Data)std::pow(2, value.front()) - 1;
            }
            else {
                maxVal = value.front();
            }
        }
        constrain = [](MLVP::Type::Data in) { return true; };
        postHandler = [](MLVP::Type::Data in) { return in; };
    }
    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType, MLVP::Type::SerialData inValue, bool isBitWidth, std::optional<std::function<bool(MLVP::Type::Data)>> inConstrain, std::optional<std::function<MLVP::Type::Data(MLVP::Type::Data)>> inPostHandler) : portName(std::move(inPortName)), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType), value(std::move(inValue)) {
        if (generatorType == GeneratorType::RANDOM_GENERATOR) {
            if (isBitWidth) {
                maxVal = (MLVP::Type::Data)std::pow(2, value.front()) - 1;
            }
            else {
                maxVal = value.front();
            }
        }
        if (inConstrain.has_value()) {
            constrain = inConstrain.value();
        }
        else {
            constrain = [](MLVP::Type::Data in) { return true; };
        }
        if (inPostHandler.has_value()) {
            postHandler = inPostHandler.value();
        }
        else {
            postHandler = [](MLVP::Type::Data in) { return in; };
        }
    }
    ~PortTestSpec() = default;
};

class PortSpecGeneratorModel {
private:
    std::shared_ptr<GeneratedUserTest> middleContents;
    std::unordered_map<std::string, PortTestSpecDefault> portTestSpecDefaults; //! Defailt Spec for each port, we strongly recommend you to set this to avoid mistakes and undefined behaviors
    std::unordered_map<std::string, std::vector<PortTestSpec>> portTestSpecs; //! Detailed Spec for each port, will cover the default settings
    int size;

public:
    PortSpecGeneratorModel() = delete;
    explicit PortSpecGeneratorModel(const std::shared_ptr<GeneratedUserTest>&inMiddleContents) : size(0), middleContents(inMiddleContents) {}
    explicit PortSpecGeneratorModel(int inSize, const std::shared_ptr<GeneratedUserTest>&inMiddleContents) : size(inSize), middleContents(inMiddleContents) {}
    ~PortSpecGeneratorModel() = default;

    bool setSize(int inSize);

    bool addPortTestSpecDefault(const std::string &inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue);

    /**
     * @brief generate default spec for transaction port
     * 
     * @param inPortName transaction port name
     * @param inGeneratorType refer to
     * ```cpp
     * enum class GeneratorType {
     *      DIRECT_INPUT = 0,
     *      RANDOM_GENERATOR = 1
     *  };
     * ```
     * @param inValue if generator type is DIRECT_INPUT, this value will be used as default value, else it will be seen as the <b>width</b> of this port, convert with verilog [max : 0]'s max + 1
     * @param inConstrain constrain, if value not satisfy, generate again
     * @param inPostHandler postHandler function of value
     * @return true pass check
     * @return false not pass check
     */
    bool addPortTestSpecDefault(const std::string &inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue, std::optional<std::function<bool(MLVP::Type::Data)>> inConstrain, std::optional<std::function<MLVP::Type::Data(MLVP::Type::Data)>> inPostHandler);

    /**
     * @brief Check PortTestSpec validity
     * 
     * @param portTestSpec 
     * @return true valid
     * @return false invalid
     */
    bool checkPortSpec(PortTestSpec &portTestSpec);

    /**
     * @brief Check PortTestSpec validity, splite struct above
     * 
     * @param portName 
     * @param startIndex 
     * @param endIndex 
     * @param generatorType 
     * @return true valid
     * @return false invalid
     */
    bool checkPortSpec(const std::string &portName, int startIndex, int &endIndex, GeneratorType generatorType);

    /**
     * @brief check all port spec
     * 
     * @return true 
     * @return false 
     */
    bool checkAllPortSpec();

    /**
     * @brief add detailed spec for transaction port
     * @details 
     * 1. index must be in order
     * 2. not have to cover whole range, use default value for the rest flexibly
     * 
     * @param portName transaction port name, <b>take effect in [startIndex, endIndex]</b>
     * @param startIndex from which index in serialTest
     * @param endIndex to which index in serialTest, <b>include</b>
     * @param generatorType GeneratorType
     * @param value 
     * - if generator type is DIRECT_INPUT, this value will be used as default value, 
     * - if you want to set one value for all, just use the first element of the vector
     * - if generator type is RANDOM_GENERATOR the first element will be seen as 
     * @return true 
     * @return false 
     */
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, const MLVP::Type::SerialData &value, bool isBitWidth);

    /**
     * @brief add detailed spec for transaction port
     * @details 
     * 1. index must be in order
     * 2. not have to cover whole range, use default value for the rest flexibly
     * 
     * @param portName transaction port name, <b>take effect in [startIndex, endIndex]</b>
     * @param startIndex from which index in serialTest
     * @param endIndex to which index in serialTest, <b>include</b>
     * @param generatorType GeneratorType
     * @param value if generator type is DIRECT_INPUT, this value will be used as default value, else the first element will be used as <b>range max + 1 in random generator</b>
     * @param inConstrain constrain, if value not satisfy, generate again
     * @param inPostHandler postHandler function of value
     * @return true 
     * @return false 
     */
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, const MLVP::Type::SerialData &value, bool isBitWidth, std::optional<std::function<bool(MLVP::Type::Data)>> inConstrain, std::optional<std::function<MLVP::Type::Data(MLVP::Type::Data)>> inPostHandler);

    /**
     * @brief Generate serial test according to the spec
     * 
     * @param autoclear Set true to clear Generator tmp storage(clearSerialTest) after execution
     */
    void generateSerialTest(bool autoclear = true);

    /**
     * @brief clear Generator tmp storage, must be done after 1 complete SerialTest setting
     * 
     */
    void clearSerialTest() { portTestSpecs.clear(); size = 0; }

};

} // namespace MLVP::TestGenerator