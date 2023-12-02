/**
 * @file main.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

//! <b>Replace all the following with your own DUT module, Notice all the stuff begin with <u>Your</u></b>

#include "mlvp.h"
#include "MCVPack/BareDut/YourModule/YourModule.h"


// ========================= DutUnitDriver =========================
class DutYourModuleDriver : public DutUnitDriver {
private:
    const shared_ptr<YourModuleTop> top;

public:
    DutYourModuleDriver(int inResetCycles, int inDriverID, const string& inLogPath) : DutUnitDriver(inResetCycles, "cache", inDriverID, inLogPath), top(make_shared<Vnutshellcache>(contextp.get(), "top")) {
        contextp->debug(0);
        //! [notice] > randReset(other value) can cause error, because our first posedge is delayed 1 cycle
        contextp->randReset(0);
        top->clock = 1;
        top->trace(tfp.get(), 99);
        //! this should be called after trace
        tfp->open((logPath + "/cache.vcd").c_str());
    }

    void reset() override {}

    bool drivingStep(bool isLast) override {
        //! Sync Global Timer
        GlobalUserTimer::getInstance().incTime();

        bool transactionFinished = false;

        if(!contextp->gotFinish()) {
            // Your implementation, connect data in transactions and channel to DUT
            //! Write your state machine here, <b>Notice that we will detect when your transaction totally done to launch new one, you MUST figure out what is transaction in your case clearly</b>
        }
        if (isLast && transactionFinished) {
            //! don't know why tfp dump lose last cycle
            contextp->timeInc(1);
            top->clock = !top->clock;
            top->eval();
            tfp->dump(contextp->time());
            contextp->timeInc(1);
            top->clock = !top->clock;
            top->eval();
            tfp->dump(contextp->time());
            //! output coverage
            Verilated::mkdir(logPath.c_str());
            contextp->coveragep()->write((logPath + "/coverage.dat").c_str());
            tfp->close();
            top->final();
        }
        return true;
    }

};

// ========================= Ref and RefDriver =========================
/**
 * @brief Ref YourModule simulator, if you hope to drive by cycles, use it
 * 
 */
//! class RefYourModule : public Ref {};

/**
 * @brief RefUnitDriver, use a ref class cache simulate the dut logic
 * @details if you hope to drive by cycles, use it as a wrapper of RefUnit, or else, just handle the transactions here
 * 
 */
class RefYourModuleDriver : public RefUnitDriver {
private:

public:
    explicit RefYourModuleDriver(int inResetCycles) : RefUnitDriver(inResetCycles, "cache") {};
    ~RefYourModuleDriver() override = default;

    void reset() override {}

    bool drivingStep(bool isLast) override {
        //! Sync Global Timer
        GlobalUserTimer::sync();

        //! Your implementation of Ref

        return true;
    }
};


// ============================= Simulator and Simulator Driver =================================

/**
 * @brief Simulator YourSimu, Notice that Ref and Dut reuse same Simulator class
 * 
 */
class SimulatorYourSimu : public Simulator {
private:

public:
    SimulatorMemory() = delete;
    ~SimulatorMemory() override = default;
    SimulatorYourSimu(bool inConnectToRef, ChannelsType &inChannels, const shared_ptr<Transaction>& inTransaction) : Simulator(inConnectToRef, "memory", inChannels, inTransaction), self("memory"), cache("cache") {
        //! You cannot set channel data here, since channels has not been established, we do this in Driver class
    }

    bool exec() override {
        //! Your implementation of Simulator

        return true;
    }

}


class SimulatorYourSimuDriver: public SimulatorDriver {
private:
    SimulatorYourSimu yourSimu;

public:
    SimulatorYourSimuDriver() = delete;
    ~SimulatorYourSimuDriver() override = default;
    explicit SimulatorYourSimuDriver(int inResetCycles, bool inConnectToRef) : SimulatorDriver(inResetCycles, inConnectToRef, "memory", make_shared<SimulatorMemory>(false, channels, transaction)), yourSimu(inConnectToRef, channels, transaction) {}

    void reset() override {}

    bool drivingStep(bool isLast) override {
        //! Sync Global Timer
        GlobalUserTimer::sync();
        return yourSimu.exec();
    }

};

// ===================== Generate Tests =====================
/**
 * @brief You can use an enum tests functions
 * 
 */
enum class TestPoint {
    DEFAULT
};

/**
 * @brief We recommend you use a function cache encapsulation your tests generation
 * 
 * @param testPoint 
 * @return const shared_ptr<SerialTestSet>& 
 */
void generateTest(PortSpecGeneratorModel &model,TestPoint testPoint) {
    //! add ports default spec
    //! add ports detailed spec, using post handler is more recommended than the constrains
    switch (testPoint) {
    }
    model.generateSerialTest(true);
}

// ========================== Evaluator ============================
/**
 * @brief You can use a function to register transaction data evaluators as you need
 * 
 */
void registerEvaluator() {
    //! Use evaluator::getInstance().registerEval funtion, check Evaluator/evaluate.h
}

int main() {
    //! instance the models
    shared_ptr<GeneratedUserTest> userTests = make_shared<GeneratedUserTest>();
    PortSpecGeneratorModel model(userTests);
    generateTest(model, TestPoint::DEFAULT);
    TransactionLauncher::setupTransaction(userTests->getTests());
    std::vector<std::pair<std::shared_ptr<SimulatorDriver>, std::shared_ptr<SimulatorDriver>>> simuDrivers = {
        {make_pair(make_shared<SimulatorYourSimuDriver>(1, false), make_shared<SimulatorYourSimuDriver>(1, true))} //! , ...
    };
    SimulatorlDriverRegistrar::getInstance().registerSimulatorDriver(simuDrivers);
    Spreader<DutYourModuleDriver, RefYourModuleDriver, VerilatorReporter> spreader(10, "log/yourModule", "report/yourModule", {
        make_pair(unordered_map<string, shared_ptr<DriverModel>>({
            {"yourSimuName", simuDrivers[0].first} //! , ...
        }), unordered_map<string, shared_ptr<DriverModel>>({
            {"yourSimuName", simuDrivers[0].second} //! , ...
        }))
    });
    //! set synchronize timer, we use GlobalUserTimer for example, you can use GlobalMachineTimer as alternative, check Library/utils.h
    //! if you use GlobalMachineTimer, use this to initialize interval (default 10 ns)
    // GlobalMachineTimer::getInstance().setUnitTimeInterval(100);
    spreader.reset();
    spreader.execute();
    return 0;
}