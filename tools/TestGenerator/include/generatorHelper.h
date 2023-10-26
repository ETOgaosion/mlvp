#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>

#include "Database/designPorts.h"
#include "Library/error.h"

namespace MVM {
namespace TestGenerator {

static const std::string testSetFilePath = "include/Sequencer/usertests.h";

static const std::string testSetFileHeader = 
R"(#pragma once

#include <vector>
#include <memory>

#include \"Sequencer/testcases.h\"

namespace MVM {
namespace Sequencer {
const static SerialTestsSet userTest = {)";

static const std::string testFileTail = 
R"(};

} // namespace Sequencer
    
} // namespace MVM)";

class GeneratorMiddleContents {
private:
    std::vector<std::vector<std::vector<int>>> userTest;
    std::mutex userTestMutex;
    std::string testStringBuffer;

public:
    GeneratorMiddleContents() : testSetIndex(0), testIndex(0), testStringBuffer("") {}
    ~GeneratorMiddleContents() = default;

    static bool checkTestValidity(std::vector<int> test);

    bool generateTestSetPrepare();
    void generateTestSetFinish();

    bool addTestSet(std::vector<std::vector<int>> testSet);
    bool addTestSet();
    bool addTest(std::vector<int> test);
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

    virtual bool addTestSet(std::vector<std::vector<int>> testSet) { return middleContents->addTestSet(testSet); }
    virtual bool addTestSet() { return middleContents->addTestSet(); }

    virtual bool addTest(std::vector<int> test) { return middleContents->addTest(test); }
    virtual bool addTest() { MVM::Library::FunctionNotImplementError("GeneratorModel::addTest"); return false; }

};

class DirectInputModel : public GeneratorModel {
private:

public:
    DirectInputModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : GeneratorModel(middleContents), generatorType(GeneratorType::DIRECT_INPUT) {}
    ~DirectInputModel() = default;
};

class RandomGeneratorModel : public GeneratorModel {
private:

public:
    RandomGeneratorModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : GeneratorModel(middleContents), generatorType(GeneratorType::RANDOM_GENERATOR) {}
    ~RandomGeneratorModel() = default;

    bool addTest(std::vector<int> test) override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addTest"); }

    bool addTestSet(std::vector<std::vector<int>> testSet) override { MVM::Library::FunctionNotImplementError("RandomGeneratorModel::addTestSet"); }

    bool addTest() override;
    bool addTestSet() override;
    
    bool addTestSets(int testSetNum);
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

class PortSpecGeneratoorModel : public GeneratorModel {
private:
    std::vector<PortTestSpec> portTestSpecs;

public:
    PortSpecGeneratoorModel(std::shared_ptr<GeneratorMiddleContents> middleContents) : GeneratorModel(middleContents), generatorType(GeneratorType::PORT_SPEC_GENERATOR) {}
    ~PortSpecGeneratoorModel() = default;

    bool addPortTestSpec(PortTestSpec portTestSpec);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType);
    bool addPortTestSpec(std::string portName, int startIndex, int endIndex, GeneratorType generatorType, std::vector<unsigned long long> value);

    void generateTest();

};

class GeneratorHelper
{
private:
    std::shared_ptr<GeneratorMiddleContents> middleContents;

public:
    GeneratorHelper() : middleContents(std::make_shared<GeneratorMiddleContents>()) {}
    ~GeneratorHelper() = default;

    bool generateTestSetPrepare() { return middleContents->generateTestSetPrepare(); }
    void generateTestSetFinish() { middleContents->generateTestSetFinish(); }

    bool addTestSet(std::vector<std::vector<int>> testSet) { return middleContents->addTestSet(testSet); }
    bool addTest(std::vector<int> test) { return middleContents->addTest(test); }

};

} // namespace TestGenerator

} // namespace MVM