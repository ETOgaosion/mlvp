#include "MCVPack/BareDut/Memory/memoryDriver.h"

#include <iostream>

using namespace std;
using namespace MVM::MCVPack;

bool DutMemoryDriver::drivingStep() {
    if(!contextp->gotFinish() && executeCycles != transaction->getMaxCycles() && testPtr <= transaction->getTestsSize()) {
        executeCycles++;
        contextp->timeInc(1);

        // assign input signals
        top->clk = !top->clk;
        top->reset = transaction->getInSignal()[testPtr][0];
        top->addr = transaction->getInSignal()[testPtr][1];
        top->wr_en = transaction->getInSignal()[testPtr][2];
        top->rd_en = transaction->getInSignal()[testPtr][3];
        top->wdata = transaction->getInSignal()[testPtr][4];

        // evaluate model
        top->eval();
        // generate trace
        tfp->dump(contextp->time());

        // assign output signals
        transaction->setDutOutSignal(testPtr, {top->rdata});
        
        testPtr++;

        if (testPtr == transaction->getTestsSize() - 1) {
            // don't know why tfp dump lose last cycle
            contextp->timeInc(1);
            top->clk = !top->clk;
            top->eval();
            tfp->dump(contextp->time());
            contextp->timeInc(1);
            top->clk = !top->clk;
            top->eval();
            tfp->dump(contextp->time());
            // output coverage
            Verilated::mkdir(logPath.c_str());
            contextp->coveragep()->write((logPath + "/coverage.dat").c_str());
            tfp->close();
            top->final();
        }
        return true;
    }
    // reach test end
    return false;
}