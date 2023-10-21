#include "RefPack/Memory/memoryDriver.h"
#include "RefPack/Memory/memory.h"

using namespace std;
using namespace MVM::RefPack;

bool RefMemoryDriver::drivingStep() {
    if (testPtr < transaction->getTestsSize()) {
        executeCycles++;
        time++;
        
        // assign input signals
        top->clk = !top->clk;
        if (!top->clk) {
            if (time > 1 && time < 10) {
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
        return false;
    }
}
