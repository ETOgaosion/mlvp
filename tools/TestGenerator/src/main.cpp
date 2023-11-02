#include "generatorHelper.h"

using namespace std;
using namespace MVM::TestGenerator;
using namespace MVM::Sequencer;
using namespace MVM::Library;

int main() {
    std::shared_ptr<GeneratorMiddleContents> middleContents = std::make_shared<GeneratorMiddleContents>();
    middleContents->generateTestSetPrepare();

    PortSpecGeneratorModel portSpecGeneratorModel(middleContents);
    for (unsigned long long i = 1; i < 5; i++) {
        portSpecGeneratorModel.addPortTestSpec("reset", 0, 1, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("addr" , 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.addPortTestSpec("wr_en", 2, 3, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("rd_en", 4, 5, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("wdata", 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.generateSerialTest(true);
    }

    middleContents->generateMiddleContents();
    middleContents->generateTestSetFinish();
    return 0;
}