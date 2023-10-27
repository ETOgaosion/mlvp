#include <string>
#include <memory>

#include "Database/designPorts.h"
#include "Database/database.h"
#include "Transaction/transaction.h"
#include "Sequencer/testcases.h"
#include "Sequencer/usertests.h"
#include "Sequencer/sequencer.h"
#include "Spreader/spreader.h"
#include "MCVPack/BareDut/Memory/memoryDriver.h"
#include "RefPack/Memory/memoryDriver.h"

using namespace std;
using namespace MVM::Database;
using namespace MVM::Sequencer;
using namespace MVM::Spreader;
using namespace MVM::Transaction;
using namespace MVM::Reporter;
using namespace MVM::MCVPack;
using namespace MVM::RefPack;

int main() {
    std::shared_ptr<TestCaseSet> testCaseSet = std::make_shared<TestCaseSet>();
    UserTest userTest(testCaseSet);
    userTest.generateUserTest("data/usertest.json");
    std::shared_ptr<Sequencer> sequencer = std::make_shared<Sequencer>(userTest.getUserTest());
    TransactionLauncher::setupTransaction(1000, sequencer);
    Spreader<DutMemoryDriver, RefMemoryDriver, VerilatorReporter> spreader("log/memory", "report/memory");
    spreader.execute();
    return 0;
}