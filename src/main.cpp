#include <string>
#include <memory>

#include "Database/designPorts.h"
#include "Database/database.h"
#include "Transaction/transaction.h"
#include "Sequencer/testcases.h"
#include "Sequencer/usertests.h"
#include "Sequencer/sequencer.h"
#include "TestGenerator/generatorHelper.h"
#include "Spreader/spreader.h"
#include "MCVPack/BareDut/Memory/memoryDriver.h"
#include "RefPack/Memory/memoryDriver.h"

using namespace std;
using namespace MVM::Type;
using namespace MVM::Database;
using namespace MVM::Sequencer;
using namespace MVM::Spreader;
using namespace MVM::TestGenerator;
using namespace MVM::Transaction;
using namespace MVM::Reporter;
using namespace MVM::MCVPack;
using namespace MVM::RefPack;

int main() {
    // Generate Tests
    std::shared_ptr<GeneratedUserTest> userTests = std::make_shared<GeneratedUserTest>();

    PortSpecGeneratorModel portSpecGeneratorModel(userTests);
    for (unsigned long long i = 1; i < 5; i++) {
        portSpecGeneratorModel.addPortTestSpec("reset", 0, 1, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("addr" , 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.addPortTestSpec("wr_en", 2, 3, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("rd_en", 4, 5, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("wdata", 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.generateSerialTest(true);
    }


    // Execution
    std::shared_ptr<TestCaseSet> testCaseSet = std::make_shared<TestCaseSet>(userTests);
    UserTest userTest(testCaseSet);
    std::shared_ptr<Sequencer> sequencer = std::make_shared<Sequencer>(userTest.getUserTest());
    TransactionLauncher::setupTransaction(1000, sequencer);
    Spreader<DutMemoryDriver, RefMemoryDriver, VerilatorReporter> spreader("log/memory", "report/memory");
    spreader.execute();
    return 0;
}