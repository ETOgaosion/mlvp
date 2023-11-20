// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_ARBITER_H_
#define VERILATED_VNUTSHELLCACHE_ARBITER_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_Arbiter final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(__PVT__io_in_0_valid,0,0);
    VL_IN8(__PVT__io_in_0_bits_setIdx,6,0);
    VL_IN8(__PVT__io_in_0_bits_waymask,3,0);
    VL_IN8(__PVT__io_in_1_valid,0,0);
    VL_IN8(__PVT__io_in_1_bits_setIdx,6,0);
    VL_IN8(__PVT__io_in_1_bits_data_dirty,0,0);
    VL_IN8(__PVT__io_in_1_bits_waymask,3,0);
    VL_OUT8(__PVT__io_out_valid,0,0);
    VL_OUT8(__PVT__io_out_bits_setIdx,6,0);
    VL_OUT8(__PVT__io_out_bits_data_dirty,0,0);
    VL_OUT8(__PVT__io_out_bits_waymask,3,0);
    CData/*0:0*/ __PVT__grant_1;
    CData/*0:0*/ __Vtogcov__io_in_0_valid;
    CData/*6:0*/ __Vtogcov__io_in_0_bits_setIdx;
    CData/*3:0*/ __Vtogcov__io_in_0_bits_waymask;
    CData/*0:0*/ __Vtogcov__io_in_1_valid;
    CData/*6:0*/ __Vtogcov__io_in_1_bits_setIdx;
    CData/*0:0*/ __Vtogcov__io_in_1_bits_data_dirty;
    CData/*3:0*/ __Vtogcov__io_in_1_bits_waymask;
    CData/*0:0*/ __Vtogcov__io_out_valid;
    CData/*6:0*/ __Vtogcov__io_out_bits_setIdx;
    CData/*0:0*/ __Vtogcov__io_out_bits_data_dirty;
    CData/*3:0*/ __Vtogcov__io_out_bits_waymask;
    CData/*0:0*/ __Vtogcov__grant_1;
    VL_IN(__PVT__io_in_0_bits_data_tag,18,0);
    VL_IN(__PVT__io_in_1_bits_data_tag,18,0);
    VL_OUT(__PVT__io_out_bits_data_tag,18,0);
    IData/*18:0*/ __Vtogcov__io_in_0_bits_data_tag;
    IData/*18:0*/ __Vtogcov__io_in_1_bits_data_tag;
    IData/*18:0*/ __Vtogcov__io_out_bits_data_tag;

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_Arbiter(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_Arbiter();
    VL_UNCOPYABLE(Vnutshellcache_Arbiter);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
