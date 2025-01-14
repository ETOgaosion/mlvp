/**
 * @file memoryMain.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "mlvp.h"
#include "MCVPack/BareDut/Memory/Vmemory.h"

// ========================= Announcer =========================

class DutMemoryDriver : public DutUnitDriver {
private:
    const shared_ptr<Vmemory> top;

public:
    DutMemoryDriver(int inDriverID, string inLogPath, shared_ptr<Transaction> inTransaction) : DutUnitDriver(inDriverID, inLogPath, inTransaction), top(std::make_shared<Vmemory>(contextp.get(), "top")) {
        contextp->debug(0);
        //! [notice] > randReset(other value) can cause error, because our first posedge is delayed 1 cycle
        contextp->randReset(0);
        top->clk = 1;
        top->trace(tfp.get(), 99);
        //! this should be called after trace
        tfp->open((logPath + "/memory.vcd").c_str());
    }

    /* just execute one test */
    bool drivingStep(bool isLast) override {
        if(!contextp->gotFinish()) {
            contextp->timeInc(1);

            //! assign input signals
            top->clk = !top->clk;
            top->reset = transaction->getInSignal()[testPtr][0];
            top->addr = transaction->getInSignal()[testPtr][1];
            top->wr_en = transaction->getInSignal()[testPtr][2];
            top->rd_en = transaction->getInSignal()[testPtr][3];
            top->wdata = transaction->getInSignal()[testPtr][4];

            //! evaluate model
            top->eval();
            //! generate trace
            tfp->dump(contextp->time());

            //! assign output signals
            transaction->setDutOutSignal(testPtr, {top->rdata});
            
            testPtr++;

            if (testPtr == transaction->getTestsSize() - 1) {
                //! don't know why tfp dump lose last cycle
                contextp->timeInc(1);
                top->clk = !top->clk;
                top->eval();
                tfp->dump(contextp->time());
                contextp->timeInc(1);
                top->clk = !top->clk;
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
        //! reach test end
        return false;
    }
};

// ========================= Verifier =========================
class RefMemory : public Ref {
private:
	/* USER JOIN TODO */
    unsigned short mem[4];

public:
    Data reset;
    Data addr;
    Data wr_en;
    Data rd_en;
    Data wdata;
    Data rdata;

    RefMemory() {
        memset(mem, 0, sizeof(mem));   
    }
    void exec() override {
		/* USER JOIN TODO */
        if (reset) {
            memset(mem, 0, sizeof(mem));
            rdata = 0;
        }
        else {
            if (wr_en) {
                mem[addr] = wdata;
            }
            if (rd_en) {
                rdata = mem[addr];
            }
        }
	}
};


class RefMemoryDriver : public RefUnitDriver {
private:
    shared_ptr<RefMemory> top;

public:
    RefMemoryDriver() = delete;
    ~RefMemoryDriver() = default;
    RefMemoryDriver(shared_ptr<Transaction> inTransaction) : RefUnitDriver(inTransaction), top(make_shared<RefMemory>()) {}

    /* just execute one test */
    bool drivingStep(bool isLast) override {
        //! assign input signals
        top->reset = transaction->getInSignal()[testPtr][0];
        top->addr = transaction->getInSignal()[testPtr][1];
        top->wr_en = transaction->getInSignal()[testPtr][2];
        top->rd_en = transaction->getInSignal()[testPtr][3];
        top->wdata = transaction->getInSignal()[testPtr][4];

        //! evaluate model
        top->eval();

        //! assign output signals
        transaction->setRefOutSignal(testPtr, {top->rdata});
        return true;
    }
};

int main() {
    //! Generate Tests
    shared_ptr<GeneratedUserTest> userTests = make_shared<GeneratedUserTest>();

    PortSpecGeneratorModel portSpecGeneratorModel(userTests);
    for (Data i = 1; i < 5; i++) {
        portSpecGeneratorModel.setSize(6);
        portSpecGeneratorModel.addPortTestSpec("addr" , 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.addPortTestSpec("wr_en", 2, 3, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("rd_en", 4, 5, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("wdata", 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.generateSerialTest(true);
    }


    //! Execution
    TransactionLauncher::setupTransaction(1000, userTests->getTests());
    Spreader<DutMemoryDriver, RefMemoryDriver, VerilatorReporter> spreader("log/memory", "report/memory");
    spreader.execute();
    return 0;
}