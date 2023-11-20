// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_ARBITER_2_H_
#define VERILATED_VNUTSHELLCACHE_ARBITER_2_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_Arbiter_2 final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_OUT8(__PVT__io_in_0_ready,0,0);
    VL_IN8(__PVT__io_in_0_valid,0,0);
    VL_IN8(__PVT__io_in_0_bits_setIdx,6,0);
    VL_IN8(__PVT__io_out_ready,0,0);
    VL_OUT8(__PVT__io_out_valid,0,0);
    VL_OUT8(__PVT__io_out_bits_setIdx,6,0);
    CData/*0:0*/ __Vtogcov__io_in_0_ready;
    CData/*0:0*/ __Vtogcov__io_in_0_valid;
    CData/*6:0*/ __Vtogcov__io_in_0_bits_setIdx;
    CData/*0:0*/ __Vtogcov__io_out_ready;
    CData/*0:0*/ __Vtogcov__io_out_valid;
    CData/*6:0*/ __Vtogcov__io_out_bits_setIdx;

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_Arbiter_2(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_Arbiter_2();
    VL_UNCOPYABLE(Vnutshellcache_Arbiter_2);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
