#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "Database/designPorts.h"
#include "MCVPack/BareDut/Memory/memoryDriver.h"
#include "RefPack/Memory/memoryDriver.h"
#include "Driver/driverModel.h"
#include "Transaction/transaction.h"
#include "MCVPack/dutDriver.h"
#include "RefPack/refDriver.h"

namespace MVM {
namespace Driver {
template <class TDut, class TRef>
class Driver {
private:
    std::unique_ptr<DriverModel> dutDriver;
    std::unique_ptr<DriverModel> refDriver;
    std::shared_ptr<MVM::Transaction::Transaction> transaction;

public:
    
    Driver(int inDriverID, std::string inLogPath, std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : transaction(inTransaction), dutDriver(std::make_unique<MVM::MCVPack::DutDriver<TDut>>(inDriverID, inLogPath, inTransaction)), refDriver(std::make_unique<MVM::RefPack::RefDriver<TRef>>(inTransaction)) {}

    int driving(std::shared_ptr<std::string> errorMsgRaw) {
        for (int i = 0; i < transaction->getTestsSize(); i++) {
            if (USE_THREADS) {
                std::thread dutThread(&MVM::Driver::DriverModel::drivingStep, dutDriver.get());
                std::thread refThread(&MVM::Driver::DriverModel::drivingStep, refDriver.get());
                dutThread.join();
                refThread.join();
            }
            else {
                dutDriver->drivingStep();
                refDriver->drivingStep();
            }
            if (!transaction->compareRefDut(i)) {
                std::string *errorMsg = errorMsgRaw.get();
                *errorMsg = "[ERROR] Driver > Test " + std::to_string(i) + " failed, input: (";
                for (int j = 0; j < MVM::Database::DesignPorts::getInstance().getPortsInSize(); j++) {
                    *errorMsg += std::to_string(transaction->getInSignal()[i][j]) + " ";
                }
                *errorMsg += "), dut output: (";
                for (int j = 0; j < MVM::Database::DesignPorts::getInstance().getPortsOutSize(); j++) {
                    *errorMsg += std::to_string(transaction->getDutOutSignal()[i][j]) + " ";
                }
                *errorMsg += "), ref output: (";
                for (int j = 0; j < MVM::Database::DesignPorts::getInstance().getPortsOutSize(); j++) {
                    *errorMsg += std::to_string(transaction->getRefOutSignal()[i][j]) + " ";
                }
                return i;
            }
        }
        return -1;
    }


};

    
} // namespace Driver

    
} // namespace MVM
