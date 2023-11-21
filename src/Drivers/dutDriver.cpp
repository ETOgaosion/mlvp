#include "Drivers/dutDriver.h"

#include <algorithm>
#include <bits/ranges_algo.h>

#include "Drivers/simulatorDriver.h"

using namespace std;
using namespace MVM::Driver;
using namespace MVM::Transaction;

bool DutTransDriver::setTransaction(shared_ptr<MVM::Transaction::Transaction> inTransaction) {
    if (!DriverModel::setTransaction(inTransaction)) {
        throw runtime_error("Transaction is not finished yet");
        return false;
    }
    if (dut->setTransaction(inTransaction)) {
        throw runtime_error("Transaction is not finished yet");
        return false;
    }
    return ranges::all_of(simulatorNames, [this, inTransaction](auto &simulatorName) {
        if (!SimulatorlDriverRegistrar::getInstance().getSimulatorDriver(simulatorSetIndex, false, simulatorName)->setTransaction(inTransaction)) {
            return false;
        }
        return true;
    });
}