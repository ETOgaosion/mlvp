/**
 * @file generatorHelper.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <utility>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <random>
#include <unordered_map>
#include <functional>

#include "Library/error.h"
#include "Library/types.h"

namespace MLVP::TestGenerator {

class GeneratedUserTest {
private:
    std::shared_ptr<MLVP::Type::SerialTestSet> userTest;
    std::mutex userTestMutex;

public:
    GeneratedUserTest() : userTest(std::make_unique<MLVP::Type::SerialTestSet>()) {}
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
    GeneratorType generatorType;
    MLVP::Type::Data value;
    std::function<MLVP::Type::Data(MLVP::Type::Data)> postHandler;

    PortTestSpecDefault() = delete;
    PortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue) : portName(std::move(inPortName)), generatorType(inGeneratorType), value(inValue) { postHandler = [](MLVP::Type::Data in) { return in; }; }
    PortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue, std::function<MLVP::Type::Data(MLVP::Type::Data)> inPostHandler) : portName(std::move(inPortName)), generatorType(inGeneratorType), value(inValue), postHandler(std::move(inPostHandler)) {}
};

struct PortTestSpec {
    std::string portName;
    GeneratorType generatorType;
    int startIndex;
    int endIndex;
    std::vector<MLVP::Type::Data> value;
    std::function<MLVP::Type::Data(MLVP::Type::Data)> postHandler;

    PortTestSpec() = delete;
    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType) : portName(std::move(inPortName)), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType) { postHandler = [](MLVP::Type::Data in) { return in; }; }
    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType, std::function<MLVP::Type::Data(MLVP::Type::Data)> inPostHandler) : portName(std::move(inPortName)), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType), postHandler(std::move(inPostHandler)) {}
    ~PortTestSpec() = default;
};

class PortSpecGeneratorModel {
private:
    std::shared_ptr<GeneratedUserTest> middleContents;
    std::unordered_map<std::string, PortTestSpecDefault> portTestSpecDefaults; //!< Defailt Spec for each port, we strongly recommend you to set this to avoid mistakes and undefined behaviors
    std::unordered_map<std::string, std::vector<PortTestSpec>> portTestSpecs; //!< Detailed Spec for each port, will cover the default settings
    int size;
    std::mt19937 rng;

public:
    PortSpecGeneratorModel() = delete;
    explicit PortSpecGeneratorModel(const std::shared_ptr<GeneratedUserTest>&inMiddleContents) : size(0), middleContents(inMiddleContents), rng((std::random_device())()) {}
    ~PortSpecGeneratorModel() = default;

    bool setSize(int inSize);

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
     * @param inValue if generator type is DIRECT_INPUT, this value will be used as default value, else it will be used as **range max + 1 in random generator**
     * @return true pass check
     * @return false not pass check
     */
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
     * @param inValue if generator type is DIRECT_INPUT, this value will be used as default value, else it will be used as **range max + 1 in random generator**
     * @param inPostHandler postHandler function of value
     * @return true pass check
     * @return false not pass check
     */
    bool addPortTestSpecDefault(const std::string &inPortName, GeneratorType inGeneratorType, MLVP::Type::Data inValue, std::function<MLVP::Type::Data(MLVP::Type::Data)> inPostHandler);

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
     * @param portName transaction port name, **take effect in [startIndex, endIndex]**
     * @param startIndex from which index in serialTest
     * @param endIndex to which index in serialTest, **include**
     * @param generatorType GeneratorType
     * @param value if generator type is DIRECT_INPUT, this value will be used as default value, else the first element will be used as **range max + 1 in random generator**
     * @return true 
     * @return false 
     */
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, const MLVP::Type::SerialData &value);

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