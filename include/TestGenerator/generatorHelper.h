#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <random>
#include <unordered_map>

#include "Library/error.h"
#include "Library/types.h"

namespace MVM {
namespace TestGenerator {

class GeneratedUserTest {
private:
    std::shared_ptr<MVM::Type::SerialTestsSet> userTest;
    std::mutex userTestMutex;

public:
    GeneratedUserTest() : userTest(std::make_unique<MVM::Type::SerialTestsSet>()) {}
    ~GeneratedUserTest() = default;

    static bool checkTestValidity(MVM::Type::TestPoint test);

    bool addSerialTest(MVM::Type::SerialTest testSet);
    bool addSerialTest();
    bool addTestPoint(MVM::Type::TestPoint test);

    std::shared_ptr<MVM::Type::SerialTestsSet> & getTests() {
        return userTest;
    }

    int getTestSize() { return userTest->size(); }
    MVM::Type::TestPoint getTestPoint(int serialTestIndex, int testPointIndex) { return (*userTest)[serialTestIndex][testPointIndex]; }
};

enum class GeneratorType {
    BASE = 0,
    DIRECT_INPUT = 1,
    RANDOM_GENERATOR = 2,
    PORT_SPEC_GENERATOR = 3,
};

class GeneratorModel {
protected:
    std::shared_ptr<GeneratedUserTest> middleContents;
    GeneratorType generatorType;
    

public:
    GeneratorModel(std::shared_ptr<GeneratedUserTest> middleContents) : middleContents(middleContents), generatorType(GeneratorType::BASE) {}
    virtual ~GeneratorModel() = default;

    virtual bool addSerialTest(MVM::Type::SerialTest testSet) { return middleContents->addSerialTest(testSet); }
    virtual bool addSerialTest() { return middleContents->addSerialTest(); }

    virtual bool addTestPoint(MVM::Type::TestPoint test) { return middleContents->addTestPoint(test); }
    virtual bool addTestPoint() { MVM::Library::FunctionNotImplementError("GeneratorModel::addTestPoint"); return false; }

};

class DirectInputModel : public GeneratorModel {
private:

public:
    DirectInputModel(std::shared_ptr<GeneratedUserTest> middleContents) : GeneratorModel(middleContents) {
        generatorType = GeneratorType::DIRECT_INPUT;
    }
    ~DirectInputModel() = default;

    bool addSerialTest() override { MVM::Library::FunctionNotImplementError("DirectInputModel::addSerialTest"); return false; }
    bool addTestPoint() override { MVM::Library::FunctionNotImplementError("DirectInputModel::addTestPoint"); return false; }
};

class RandomGeneratorModel : public GeneratorModel {
private:
    std::random_device dev;
    std::mt19937 rng;

public:
    RandomGeneratorModel(std::shared_ptr<GeneratedUserTest> middleContents) : GeneratorModel(middleContents), rng((std::random_device())()) {
        generatorType = GeneratorType::RANDOM_GENERATOR;
    }
    ~RandomGeneratorModel() = default;

    bool addTestPoint(MVM::Type::TestPoint test) override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addTestPoint"); return false; }

    bool addSerialTest(MVM::Type::SerialTest testSet) override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addSerialTest"); return false; }

    bool addTestPoint() override;
    bool addSerialTest() override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addSerialTest"); return false; }
    
    bool addSerialTest(int testNum);
    bool addSerialTestsSet(std::vector<int> serialTestSize);
};

struct PortTestSpec {
    std::string portName;
    int startIndex;
    int endIndex;
    GeneratorType generatorType;
    std::vector<unsigned long long> value;

    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType) : portName(inPortName), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType) {}
    ~PortTestSpec() = default;
};

class PortSpecGeneratorModel : public GeneratorModel {
private:
    std::unordered_map<std::string, std::vector<PortTestSpec>> portTestSpecs;
    int maxIndex;
    std::mt19937 rng;

public:
    PortSpecGeneratorModel(std::shared_ptr<GeneratedUserTest> middleContents) : GeneratorModel(middleContents), maxIndex(0), rng((std::random_device())()) {
        generatorType = GeneratorType::PORT_SPEC_GENERATOR;
    }
    ~PortSpecGeneratorModel() = default;

    bool checkPortSpec(PortTestSpec portTestSpec);
    bool checkPortSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType);
    bool checkAllPortSpec();

    bool addPortTestSpec(PortTestSpec portTestSpec);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, std::vector<unsigned long long> value);

    void generateSerialTest(bool autoclear = true);
    void clearSerialTest() { portTestSpecs.clear(); maxIndex = 0; }

    bool addSerialTest(MVM::Type::SerialTest testSet) override { MVM::Library::FunctionNotImplementError("PortSpecGeneratorModel::addSerialTest"); return false; }
    bool addSerialTest() override { MVM::Library::FunctionNotImplementError("PortSpecGeneratorModel::addSerialTest"); return false; }
    bool addTestPoint(MVM::Type::TestPoint test) override { MVM::Library::FunctionNotImplementError("PortSpecGeneratorModel::addTestPoint"); return false; }

};

} // namespace TestGenerator

} // namespace MVM