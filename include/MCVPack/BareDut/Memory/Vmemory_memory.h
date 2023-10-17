// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vmemory.h for the primary calling header

#ifndef VERILATED_VMEMORY_MEMORY_H_
#define VERILATED_VMEMORY_MEMORY_H_  // guard

#include "verilated.h"


class Vmemory__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vmemory_memory final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(addr,1,0);
    VL_IN8(wr_en,0,0);
    VL_IN8(rd_en,0,0);
    VL_IN8(wdata,7,0);
    VL_OUT8(rdata,7,0);
    CData/*1:0*/ __Vdlyvdim0__mem__v0;
    CData/*7:0*/ __Vdlyvval__mem__v0;
    CData/*0:0*/ __Vdlyvset__mem__v0;
    CData/*7:0*/ __Vdly__rdata;
    VlUnpacked<CData/*7:0*/, 4> __PVT__mem;

    // INTERNAL VARIABLES
    Vmemory__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vmemory_memory(Vmemory__Syms* symsp, const char* v__name);
    ~Vmemory_memory();
    VL_UNCOPYABLE(Vmemory_memory);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
