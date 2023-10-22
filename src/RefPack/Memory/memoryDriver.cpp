#include "RefPack/Memory/memoryDriver.h"
#include "RefPack/Memory/memory.h"

using namespace std;
using namespace MVM::RefPack;

bool RefMemoryDriver::drivingStep() {
    if (testPtr <= transaction->getTestsSize()) {
        executeCycles++;
        time++;
        
        // assign input signals
        top->clk = !top->clk;
        top->reset = transaction->getInSignal()[testPtr][0];
        top->addr = transaction->getInSignal()[testPtr][1];
        top->wr_en = transaction->getInSignal()[testPtr][2];
        top->rd_en = transaction->getInSignal()[testPtr][3];
        top->wdata = transaction->getInSignal()[testPtr][4];

        // evaluate model
        top->eval();

        // assign output signals
        transaction->setRefOutSignal(testPtr, {top->rdata});
        testPtr++;
        return true;
    }
    return false;
}
