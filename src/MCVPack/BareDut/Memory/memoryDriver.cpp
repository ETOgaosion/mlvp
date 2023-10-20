#include "MCVPack/BareDut/Memory/memoryDriver.h"

#include "MCVPack/BareDut/Memory/Vmemory.h"

using namespace std;
using namespace MVM::MCVPack;

bool MemoryDriver::drivingStep() {
    if(!contextp->gotFinish() && executeCycles != transaction->getMaxCycles() && testPtr < transaction->getTestsSize()) {
        executeCycles++;
        contextp->timeInc(1);

        // assign input signals
        top->clk = !top->clk;
        if (!top->clk) {
            if (contextp->time() > 1 && contextp->time() < 10) {
                top->reset = 1;
            } else {
                top->reset = 0;
            }
        }
        top->addr = transaction->getInSignal()[testPtr][0];
        top->wr_en = transaction->getInSignal()[testPtr][1];
        top->rd_en = transaction->getInSignal()[testPtr][2];
        top->wdata = transaction->getInSignal()[testPtr][3];

        // evaluate model
        top->eval();

        // assign output signals
        transaction->setDutOutSignal(testPtr, {top->rdata});

        return true;
    }
    else {
        top->final();
        Verilated::mkdir("logs/memory");
        contextp->coveragep()->write("logs/memory/coverage.dat");
        
        return false;
    }
}