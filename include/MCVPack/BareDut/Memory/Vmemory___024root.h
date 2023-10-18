// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vmemory.h for the primary calling header

#ifndef VERILATED_VMEMORY___024ROOT_H_
#define VERILATED_VMEMORY___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"
class Vmemory_memory;


class Vmemory__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vmemory___024root final : public VerilatedModule {
  public:
    // CELLS
    Vmemory_memory* __PVT__memory;

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(addr,1,0);
    VL_IN8(wr_en,0,0);
    VL_IN8(rd_en,0,0);
    VL_IN8(wdata,7,0);
    VL_OUT8(rdata,7,0);
    CData/*0:0*/ __VstlContinue;
    CData/*0:0*/ __VicoContinue;
    CData/*0:0*/ __Vtrigprevexpr___TOP__memory__reset__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__memory__clk__0;
    CData/*0:0*/ __VactContinue;
    CData/*0:0*/ __VnbaContinue;
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VactIterCount;
    IData/*31:0*/ __VnbaIterCount;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VpreTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vmemory__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vmemory___024root(Vmemory__Syms* symsp, const char* v__name);
    ~Vmemory___024root();
    VL_UNCOPYABLE(Vmemory___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
