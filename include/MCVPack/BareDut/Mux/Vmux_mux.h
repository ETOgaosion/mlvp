// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vmux.h for the primary calling header

#ifndef VERILATED_VMUX_MUX_H_
#define VERILATED_VMUX_MUX_H_  // guard

#include "verilated.h"


class Vmux__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vmux_mux final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(sel,0,0);
    VL_IN8(a,0,0);
    VL_IN8(b,0,0);
    VL_OUT8(out,0,0);

    // INTERNAL VARIABLES
    Vmux__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vmux_mux(Vmux__Syms* symsp, const char* v__name);
    ~Vmux_mux();
    VL_UNCOPYABLE(Vmux_mux);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
