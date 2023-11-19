#include "Drivers/dutDriver.h"

#include "Drivers/simulatorDriver.h"

using namespace std;
using namespace MVM::Driver;
using namespace MVM::Transaction;

bool DutTransDriver::setTransaction(std::shared_ptr<MVM::Transaction::Transaction> inTransaction) {
    if (!DriverModel::setTransaction(inTransaction)) {
        throw std::runtime_error("Transaction is not finished yet");
        return false;
    }
    if (dut->setTransaction(inTransaction)) {
        throw std::runtime_error("Transaction is not finished yet");
        return false;
    }
    for (auto &simulatorName : simulatorNames) {
        if (!SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, simulatorName)->setTransaction(inTransaction)) {
            throw std::runtime_error("Transaction is not finished yet");
            return false;
        }
    }
    return true;
}