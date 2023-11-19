#pragma once

#include <string>
#include <memory>

#include "Drivers/driverModel.h"
#include "Transaction/transaction.h"

namespace MVM {
namespace Driver {
class RefUnitDriver : public MVM::Driver::DriverModel {
private:

protected:

public:
    RefUnitDriver() = delete;
    ~RefUnitDriver() = default;
    RefUnitDriver(std::string inUnitName) : DriverModel(inUnitName) {}

    /**
     * bool drivingStep() override
     * @brief Driving step for simulator
     * @details just execute one test
     *          no implementation will also be a pure function
     *          [announcer]|[verifier] must implement
     * 
     */
};
    
} // namespace RefPack

} // namesapce MVM