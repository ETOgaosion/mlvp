#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <random>
#include <unordered_map>

#include "Sequencer/testcases.h"
#include "Library/error.h"

namespace MVM {
namespace TestGenerator {

static const std::string testSetFilePath = "data/usertest.json";

class GeneratorMiddleContents {
private:
    MVM::Sequencer::SerialTestsSet userTest;
    std::mutex userTestMutex;

public:
    GeneratorMiddleContents() {
        userTest.clear();
    }
    ~GeneratorMiddleContents() = default;

    static bool checkTestValidity(MVM::Sequencer::TestPoint test);

    bool generateTestSetPrepare();
    void generateMiddleContents();
    void generateTestSetFinish();

    bool addSerialTest(MVM::Sequencer::SerialTest testSet);
    bool addSerialTest();
    bool addTestPoint(MVM::Sequencer::TestPoint test);
};

enum class GeneratorType {
    BASE = 0,
    DIRECT_INPUT = 1,
    RANDOM_GENERATOR = 2,
    PORT_SPEC_GENERATOR = 3,
};

class GeneratorModel {
protected:
    std::shared_ptr<GeneratorMiddleContents> middleContents;
    GeneratorType generatorType;
    

public:
    GeneratorModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : middleContents(middleContents), generatorType(GeneratorType::BASE) {}
    virtual ~GeneratorModel() = default;

    virtual bool addSerialTest(MVM::Sequencer::SerialTest testSet) { return middleContents->addSerialTest(testSet); }
    virtual bool addSerialTest() { return middleContents->addSerialTest(); }

    virtual bool addTestPoint(MVM::Sequencer::TestPoint test) { return middleContents->addTestPoint(test); }
    virtual bool addTestPoint() { MVM::Library::FunctionNotImplementError("GeneratorModel::addTestPoint"); return false; }

};

class DirectInputModel : public GeneratorModel {
private:

public:
    DirectInputModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : GeneratorModel(middleContents) {
        generatorType = GeneratorType::DIRECT_INPUT;
    }
    ~DirectInputModel() = default;
};

class RandomGeneratorModel : public GeneratorModel {
private:
    std::random_device dev;
    std::mt19937 rng;

public:
    RandomGeneratorModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : GeneratorModel(middleContents), rng((std::random_device())()) {
        generatorType = GeneratorType::RANDOM_GENERATOR;
    }
    ~RandomGeneratorModel() = default;

    bool addTestPoint(MVM::Sequencer::TestPoint test) override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addTestPoint"); return false; }

    bool addSerialTest(MVM::Sequencer::SerialTest testSet) override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addSerialTest"); return false; }

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
    MVM::Sequencer::TestPoint value;

    PortTestSpec(std::string inPortName, int inStartIndex, int inEndIndex, GeneratorType inGeneratorType) : portName(inPortName), startIndex(inStartIndex), endIndex(inEndIndex), generatorType(inGeneratorType) {}
    ~PortTestSpec() = default;
};

class PortSpecGeneratoorModel : public GeneratorModel {
private:
    std::unordered_map<std::string, std::vector<PortTestSpec>> portTestSpecs;
    int maxIndex;
    std::mt19937 rng;

public:
    PortSpecGeneratoorModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : GeneratorModel(middleContents), maxIndex(0), rng((std::random_device())()) {
        generatorType = GeneratorType::PORT_SPEC_GENERATOR;
    }
    ~PortSpecGeneratoorModel() = default;

    bool checkPortSpec(PortTestSpec portTestSpec);
    bool checkPortSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType);
    bool checkAllPortSpec();

    bool addPortTestSpec(PortTestSpec portTestSpec);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, MVM::Sequencer::TestPoint value);

    void generateSerialTest(bool autoclear = true);
    void clearSerialTest() { portTestSpecs.clear(); maxIndex = 0; }

    bool addSerialTest(MVM::Sequencer::SerialTest testSet) override { MVM::Library::FunctionNotImplementError("PortSpecGeneratoorModel::addSerialTest"); return false; }
    bool addSerialTest() override { MVM::Library::FunctionNotImplementError("PortSpecGeneratoorModel::addSerialTest"); return false; }
    bool addTestPoint(MVM::Sequencer::TestPoint test) override { MVM::Library::FunctionNotImplementError("PortSpecGeneratoorModel::addTestPoint"); return false; }

};

} // namespace TestGenerator

} // namespace MVM