#include "Drivers/refDriver.h"

#include "Drivers/simulatorDriver.h"

using namespace std;
using namespace MVM::Driver;
using namespace MVM::Transaction;

bool RefTransDriver::setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) {
    if (!DriverModel::setTransaction(inTransaction)) {
        throw std::runtime_error("Transaction is not finished yet");
        return false;
    }
    if (ref->setTransaction(inTransaction)) {
        throw std::runtime_error("Transaction is not finished yet");
        return false;
    }
    for (auto &simulatorName : simulatorNames) {
        if (!SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, true, simulatorName)->setTransaction(inTransaction)) {
            throw std::runtime_error("Transaction is not finished yet");
            return false;
        }
    }
    return true;
}