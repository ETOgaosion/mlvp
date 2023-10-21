#pragma once

#include <string>
#include <memory>

#include "Driver/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace MCVPack {
template <class T>
class DutDriver : public MVM::Driver::DriverModel {
private:
    std::unique_ptr<MVM::Driver::DriverModel> dut;

public:
    DutDriver() = delete;
    ~DutDriver() = default;
    DutDriver(int inDriverID, std::string inLogPath, std::shared_ptr<MVM::Transaction::Transaction> inTransaction) : dut(std::make_unique<T>(inDriverID, inLogPath, inTransaction)) {}

    bool drivingStep() override {
        return dut->drivingStep();
    }

};

    
} // namespace MCVPack


} // namespace MVM