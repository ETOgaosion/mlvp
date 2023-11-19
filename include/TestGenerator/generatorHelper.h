#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <random>
#include <unordered_map>
#include <functional>

#include "Library/error.h"
#include "Library/types.h"

namespace MVM {
namespace TestGenerator {

class GeneratedUserTest {
private:
    std::shared_ptr<MVM::Type::SerialTestSet> userTest;
    std::mutex userTestMutex;

public:
    GeneratedUserTest() : userTest(std::make_unique<MVM::Type::SerialTestSet>()) {}
    ~GeneratedUserTest() = default;

    void addSerialTest(MVM::Type::SerialTest testSet);
    void addSerialTest();
    void addPortTest(string portName, TestData test);

    std::shared_ptr<MVM::Type::SerialTestSet> & getTests() {
        return userTest;
    }

    int getTestSize() { return userTest->size(); }
};

enum class GeneratorType {
    DIRECT_INPUT = 0,
    RANDOM_GENERATOR = 1
};

struct PortTestSpecDefault {
    std::string portName;
    GeneratorType generatorType;
    MVM::Type::uint64 value;
    std::function<MVM::Type::uint64(MVM::Type::uint64)> postHandler;

    PortTestSpecDefault() = delete;
    PortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MVM::Type::uint64 inValue) : portName(inPortName), generatorType(inGeneratorType), value(inValue) { postHandler = [](MVM::Type::uint64 in) { return in; }; }
    PortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MVM::Type::uint64 inValue, std::function<MVM::Type::uint64(MVM::Type::uint64)> inPostHandler) : portName(inPortName), generatorType(inGeneratorType), value(inValue), postHandler(inPostHandler) {}
};

struct PortTestSpec {
    std::string portName;
    GeneratorType generatorType;
    int startIndex;
    int endIndex;
    std::vector<MVM::Type::uint64> value;
    std::function<MVM::Type::uint64(MVM::Type::uint64)> postHandler;

    PortTestSpec() = delete;
    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType) : portName(inPortName), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType) { postHandler = [](MVM::Type::uint64 in) { return in; }; }
    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType, std::function<MVM::Type::uint64(MVM::Type::uint64)> inPostHandler) : portName(inPortName), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType), postHandler(inPostHandler) {}
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
    PortSpecGeneratorModel(std::shared_ptr<GeneratedUserTest> inMiddleContents) : size(0), middleContents(middleContents), rng((std::random_device())()) {}
    ~PortSpecGeneratorModel() = default;

    bool setSize(int inSize);

    bool addPortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MVM::Type::uint64 inValue);
    bool addPortTestSpecDefault(std::string inPortName, GeneratorType inGeneratorType, MVM::Type::uint64 inValue, std::function<MVM::Type::uint64(MVM::Type::uint64)> inPostHandler);

    bool checkPortSpec(PortTestSpec &portTestSpec);
    bool checkPortSpec(std::string portName, int startIndex, int &endIndex, GeneratorType generatorType);
    bool checkAllPortSpec();

    bool addPortTestSpec(PortTestSpec portTestSpec);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, std::vector<MVM::Type::uint64> value);

    void generateSerialTest(bool autoclear = true);
    void clearSerialTest() { portTestSpecs.clear(); size = 0; }

};

} // namespace TestGenerator

} // namespace MVM