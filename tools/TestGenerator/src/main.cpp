#include "generatorHelper.h"

using namespace std;
using namespace MVM::TestGenerator;
using namespace MVM::Sequencer;
using namespace MVM::Library;

int main() {
    std::shared_ptr<GeneratorMiddleContents> middleContents = std::make_shared<GeneratorMiddleContents>();
    middleContents->generateTestSetPrepare();

    PortSpecGeneratoorModel portSpecGeneratoorModel(middleContents);
    for (unsigned long long i = 1; i < 5; i++) {
        portSpecGeneratoorModel.addPortTestSpec("reset", 0, 1, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratoorModel.addPortTestSpec("reset", 2, 5, GeneratorType::DIRECT_INPUT, {0});
        portSpecGeneratoorModel.addPortTestSpec("addr" , 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratoorModel.addPortTestSpec("wr_en", 0, 1, GeneratorType::DIRECT_INPUT, {0});
        portSpecGeneratoorModel.addPortTestSpec("wr_en", 2, 3, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratoorModel.addPortTestSpec("wr_en", 4, 5, GeneratorType::DIRECT_INPUT, {0});
        portSpecGeneratoorModel.addPortTestSpec("rd_en", 0, 3, GeneratorType::DIRECT_INPUT, {0});
        portSpecGeneratoorModel.addPortTestSpec("rd_en", 4, 5, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratoorModel.addPortTestSpec("wdata", 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratoorModel.generateSerialTest(true);
    }

    middleContents->generateMiddleContents();
    middleContents->generateTestSetFinish();
    return 0;
}