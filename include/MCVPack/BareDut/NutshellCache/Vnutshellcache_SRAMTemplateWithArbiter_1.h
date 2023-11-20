// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_SRAMTEMPLATEWITHARBITER_1_H_
#define VERILATED_VNUTSHELLCACHE_SRAMTEMPLATEWITHARBITER_1_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"
class Vnutshellcache_Arbiter_3;
class Vnutshellcache_SRAMTemplate_1;


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_SRAMTemplateWithArbiter_1 final : public VerilatedModule {
  public:
    // CELLS
    Vnutshellcache_SRAMTemplate_1* __PVT__ram;
    Vnutshellcache_Arbiter_3* __PVT__readArb;

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(__PVT__clock,0,0);
        VL_IN8(__PVT__reset,0,0);
        VL_OUT8(__PVT__io_r_0_req_ready,0,0);
        VL_IN8(__PVT__io_r_0_req_valid,0,0);
        VL_OUT8(__PVT__io_r_1_req_ready,0,0);
        VL_IN8(__PVT__io_r_1_req_valid,0,0);
        VL_IN8(__PVT__io_w_req_valid,0,0);
        VL_IN8(__PVT__io_w_req_bits_waymask,3,0);
        CData/*0:0*/ __PVT__ram_clock;
        CData/*0:0*/ __PVT__ram_io_r_req_ready;
        CData/*0:0*/ __PVT__ram_io_r_req_valid;
        CData/*0:0*/ __PVT__ram_io_w_req_valid;
        CData/*3:0*/ __PVT__ram_io_w_req_bits_waymask;
        CData/*0:0*/ __PVT__readArb_io_in_0_ready;
        CData/*0:0*/ __PVT__readArb_io_in_0_valid;
        CData/*0:0*/ __PVT__readArb_io_in_1_ready;
        CData/*0:0*/ __PVT__readArb_io_in_1_valid;
        CData/*0:0*/ __PVT__readArb_io_out_ready;
        CData/*0:0*/ __PVT__readArb_io_out_valid;
        CData/*0:0*/ __PVT__REG;
        CData/*0:0*/ __PVT__REG_1;
        CData/*0:0*/ __Vtogcov__clock;
        CData/*0:0*/ __Vtogcov__reset;
        CData/*0:0*/ __Vtogcov__io_r_0_req_ready;
        CData/*0:0*/ __Vtogcov__io_r_0_req_valid;
        CData/*0:0*/ __Vtogcov__io_r_1_req_ready;
        CData/*0:0*/ __Vtogcov__io_r_1_req_valid;
        CData/*0:0*/ __Vtogcov__io_w_req_valid;
        CData/*3:0*/ __Vtogcov__io_w_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__ram_clock;
        CData/*0:0*/ __Vtogcov__ram_io_r_req_ready;
        CData/*0:0*/ __Vtogcov__ram_io_r_req_valid;
        CData/*0:0*/ __Vtogcov__ram_io_w_req_valid;
        CData/*3:0*/ __Vtogcov__ram_io_w_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__readArb_io_in_0_ready;
        CData/*0:0*/ __Vtogcov__readArb_io_in_0_valid;
        CData/*0:0*/ __Vtogcov__readArb_io_in_1_ready;
        CData/*0:0*/ __Vtogcov__readArb_io_in_1_valid;
        CData/*0:0*/ __Vtogcov__readArb_io_out_ready;
        CData/*0:0*/ __Vtogcov__readArb_io_out_valid;
        CData/*0:0*/ __Vtogcov__REG;
        CData/*0:0*/ __Vtogcov__REG_1;
        CData/*0:0*/ __Vdly__REG;
        CData/*0:0*/ __Vdly__REG_1;
        VL_IN16(__PVT__io_r_0_req_bits_setIdx,9,0);
        VL_IN16(__PVT__io_r_1_req_bits_setIdx,9,0);
        VL_IN16(__PVT__io_w_req_bits_setIdx,9,0);
        SData/*9:0*/ __PVT__ram_io_r_req_bits_setIdx;
        SData/*9:0*/ __PVT__ram_io_w_req_bits_setIdx;
        SData/*9:0*/ __PVT__readArb_io_in_0_bits_setIdx;
        SData/*9:0*/ __PVT__readArb_io_in_1_bits_setIdx;
        SData/*9:0*/ __PVT__readArb_io_out_bits_setIdx;
        SData/*9:0*/ __Vtogcov__io_r_0_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__io_r_1_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__io_w_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__ram_io_r_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__ram_io_w_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__readArb_io_in_0_bits_setIdx;
        SData/*9:0*/ __Vtogcov__readArb_io_in_1_bits_setIdx;
        SData/*9:0*/ __Vtogcov__readArb_io_out_bits_setIdx;
        VL_OUT64(__PVT__io_r_0_resp_data_0_data,63,0);
        VL_OUT64(__PVT__io_r_0_resp_data_1_data,63,0);
        VL_OUT64(__PVT__io_r_0_resp_data_2_data,63,0);
        VL_OUT64(__PVT__io_r_0_resp_data_3_data,63,0);
    };
    struct {
        VL_OUT64(__PVT__io_r_1_resp_data_0_data,63,0);
        VL_OUT64(__PVT__io_r_1_resp_data_1_data,63,0);
        VL_OUT64(__PVT__io_r_1_resp_data_2_data,63,0);
        VL_OUT64(__PVT__io_r_1_resp_data_3_data,63,0);
        VL_IN64(__PVT__io_w_req_bits_data_data,63,0);
        QData/*63:0*/ __PVT__ram_io_r_resp_data_0_data;
        QData/*63:0*/ __PVT__ram_io_r_resp_data_1_data;
        QData/*63:0*/ __PVT__ram_io_r_resp_data_2_data;
        QData/*63:0*/ __PVT__ram_io_r_resp_data_3_data;
        QData/*63:0*/ __PVT__ram_io_w_req_bits_data_data;
        QData/*63:0*/ __PVT__r___05F0_data;
        QData/*63:0*/ __PVT__r___05F1_data;
        QData/*63:0*/ __PVT__r___05F2_data;
        QData/*63:0*/ __PVT__r___05F3_data;
        QData/*63:0*/ __PVT__r_1_0_data;
        QData/*63:0*/ __PVT__r_1_1_data;
        QData/*63:0*/ __PVT__r_1_2_data;
        QData/*63:0*/ __PVT__r_1_3_data;
        QData/*63:0*/ __Vtogcov__io_r_0_resp_data_0_data;
        QData/*63:0*/ __Vtogcov__io_r_0_resp_data_1_data;
        QData/*63:0*/ __Vtogcov__io_r_0_resp_data_2_data;
        QData/*63:0*/ __Vtogcov__io_r_0_resp_data_3_data;
        QData/*63:0*/ __Vtogcov__io_r_1_resp_data_0_data;
        QData/*63:0*/ __Vtogcov__io_r_1_resp_data_1_data;
        QData/*63:0*/ __Vtogcov__io_r_1_resp_data_2_data;
        QData/*63:0*/ __Vtogcov__io_r_1_resp_data_3_data;
        QData/*63:0*/ __Vtogcov__io_w_req_bits_data_data;
        QData/*63:0*/ __Vtogcov__ram_io_r_resp_data_0_data;
        QData/*63:0*/ __Vtogcov__ram_io_r_resp_data_1_data;
        QData/*63:0*/ __Vtogcov__ram_io_r_resp_data_2_data;
        QData/*63:0*/ __Vtogcov__ram_io_r_resp_data_3_data;
        QData/*63:0*/ __Vtogcov__ram_io_w_req_bits_data_data;
        QData/*63:0*/ __Vtogcov__r___05F0_data;
        QData/*63:0*/ __Vtogcov__r___05F1_data;
        QData/*63:0*/ __Vtogcov__r___05F2_data;
        QData/*63:0*/ __Vtogcov__r___05F3_data;
        QData/*63:0*/ __Vtogcov__r_1_0_data;
        QData/*63:0*/ __Vtogcov__r_1_1_data;
        QData/*63:0*/ __Vtogcov__r_1_2_data;
        QData/*63:0*/ __Vtogcov__r_1_3_data;
        QData/*63:0*/ __Vdly__r___05F0_data;
        QData/*63:0*/ __Vdly__r___05F1_data;
        QData/*63:0*/ __Vdly__r___05F2_data;
        QData/*63:0*/ __Vdly__r___05F3_data;
        QData/*63:0*/ __Vdly__r_1_0_data;
        QData/*63:0*/ __Vdly__r_1_1_data;
        QData/*63:0*/ __Vdly__r_1_2_data;
        QData/*63:0*/ __Vdly__r_1_3_data;
    };

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_SRAMTemplateWithArbiter_1(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_SRAMTemplateWithArbiter_1();
    VL_UNCOPYABLE(Vnutshellcache_SRAMTemplateWithArbiter_1);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
