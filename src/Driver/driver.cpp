#include "Driver/driver.h"

#include <iostream>

#include <Database/designPorts.h>

using namespace std;
using namespace MVM::Driver;
using namespace MVM::MCVPack;
using namespace MVM::RefPack;
using namespace MVM::Database;

int Driver::driving(string &errorMsg) {
    for (int i = 0; i < transaction->getTestsSize(); i++) {
        thread dutThread(&DutDriver::drivingStep, dutDriver);
        thread refThread(&RefDriver::drivingStep, refDriver);
        dutThread.join();
        refThread.join();
        if (!transaction->compareRefDut(i)) {
            errorMsg = "[ERROR] Driver > Test " + to_string(i) + " failed, input: (";
            for (int j = 0; j < DesignPorts::getInstance().getPortsInSize(); j++) {
                errorMsg += to_string(transaction->getInSignal()[i][j]) + " ";
            }
            errorMsg += "), dut output: (";
            for (int j = 0; j < DesignPorts::getInstance().getPortsOutSize(); j++) {
                errorMsg += to_string(transaction->getDutOutSignal()[i][j]) + " ";
            }
            errorMsg += "), ref output: (";
            for (int j = 0; j < DesignPorts::getInstance().getPortsOutSize(); j++) {
                errorMsg += to_string(transaction->getRefOutSignal()[i][j]) + " ";
            }
            return i;
        }
    }
    return -1;
}