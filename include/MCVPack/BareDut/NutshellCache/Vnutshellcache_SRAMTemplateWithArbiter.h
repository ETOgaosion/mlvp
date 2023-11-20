// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_SRAMTEMPLATEWITHARBITER_H_
#define VERILATED_VNUTSHELLCACHE_SRAMTEMPLATEWITHARBITER_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"
class Vnutshellcache_Arbiter_2;
class Vnutshellcache_SRAMTemplate;


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_SRAMTemplateWithArbiter final : public VerilatedModule {
  public:
    // CELLS
    Vnutshellcache_SRAMTemplate* __PVT__ram;
    Vnutshellcache_Arbiter_2* __PVT__readArb;

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(__PVT__clock,0,0);
        VL_IN8(__PVT__reset,0,0);
        VL_OUT8(__PVT__io_r_0_req_ready,0,0);
        VL_IN8(__PVT__io_r_0_req_valid,0,0);
        VL_IN8(__PVT__io_r_0_req_bits_setIdx,6,0);
        VL_OUT8(__PVT__io_r_0_resp_data_0_valid,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_0_dirty,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_1_valid,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_1_dirty,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_2_valid,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_2_dirty,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_3_valid,0,0);
        VL_OUT8(__PVT__io_r_0_resp_data_3_dirty,0,0);
        VL_IN8(__PVT__io_w_req_valid,0,0);
        VL_IN8(__PVT__io_w_req_bits_setIdx,6,0);
        VL_IN8(__PVT__io_w_req_bits_data_dirty,0,0);
        VL_IN8(__PVT__io_w_req_bits_waymask,3,0);
        CData/*0:0*/ __PVT__ram_clock;
        CData/*0:0*/ __PVT__ram_reset;
        CData/*0:0*/ __PVT__ram_io_r_req_ready;
        CData/*0:0*/ __PVT__ram_io_r_req_valid;
        CData/*6:0*/ __PVT__ram_io_r_req_bits_setIdx;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_0_valid;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_0_dirty;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_1_valid;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_1_dirty;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_2_valid;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_2_dirty;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_3_valid;
        CData/*0:0*/ __PVT__ram_io_r_resp_data_3_dirty;
        CData/*0:0*/ __PVT__ram_io_w_req_valid;
        CData/*6:0*/ __PVT__ram_io_w_req_bits_setIdx;
        CData/*0:0*/ __PVT__ram_io_w_req_bits_data_dirty;
        CData/*3:0*/ __PVT__ram_io_w_req_bits_waymask;
        CData/*0:0*/ __PVT__readArb_io_in_0_ready;
        CData/*0:0*/ __PVT__readArb_io_in_0_valid;
        CData/*6:0*/ __PVT__readArb_io_in_0_bits_setIdx;
        CData/*0:0*/ __PVT__readArb_io_out_ready;
        CData/*0:0*/ __PVT__readArb_io_out_valid;
        CData/*6:0*/ __PVT__readArb_io_out_bits_setIdx;
        CData/*0:0*/ __PVT__REG;
        CData/*0:0*/ __PVT__r_0_valid;
        CData/*0:0*/ __PVT__r_0_dirty;
        CData/*0:0*/ __PVT__r_1_valid;
        CData/*0:0*/ __PVT__r_1_dirty;
        CData/*0:0*/ __PVT__r_2_valid;
        CData/*0:0*/ __PVT__r_2_dirty;
        CData/*0:0*/ __PVT__r_3_valid;
        CData/*0:0*/ __PVT__r_3_dirty;
        CData/*0:0*/ __Vtogcov__clock;
        CData/*0:0*/ __Vtogcov__reset;
        CData/*0:0*/ __Vtogcov__io_r_0_req_ready;
        CData/*0:0*/ __Vtogcov__io_r_0_req_valid;
        CData/*6:0*/ __Vtogcov__io_r_0_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_0_valid;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_0_dirty;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_1_valid;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_1_dirty;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_2_valid;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_2_dirty;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_3_valid;
        CData/*0:0*/ __Vtogcov__io_r_0_resp_data_3_dirty;
        CData/*0:0*/ __Vtogcov__io_w_req_valid;
        CData/*6:0*/ __Vtogcov__io_w_req_bits_setIdx;
    };
    struct {
        CData/*0:0*/ __Vtogcov__io_w_req_bits_data_dirty;
        CData/*3:0*/ __Vtogcov__io_w_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__ram_clock;
        CData/*0:0*/ __Vtogcov__ram_reset;
        CData/*0:0*/ __Vtogcov__ram_io_r_req_ready;
        CData/*0:0*/ __Vtogcov__ram_io_r_req_valid;
        CData/*6:0*/ __Vtogcov__ram_io_r_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_0_valid;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_0_dirty;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_1_valid;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_1_dirty;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_2_valid;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_2_dirty;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_3_valid;
        CData/*0:0*/ __Vtogcov__ram_io_r_resp_data_3_dirty;
        CData/*0:0*/ __Vtogcov__ram_io_w_req_valid;
        CData/*6:0*/ __Vtogcov__ram_io_w_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__ram_io_w_req_bits_data_dirty;
        CData/*3:0*/ __Vtogcov__ram_io_w_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__readArb_io_in_0_ready;
        CData/*0:0*/ __Vtogcov__readArb_io_in_0_valid;
        CData/*6:0*/ __Vtogcov__readArb_io_in_0_bits_setIdx;
        CData/*0:0*/ __Vtogcov__readArb_io_out_ready;
        CData/*0:0*/ __Vtogcov__readArb_io_out_valid;
        CData/*6:0*/ __Vtogcov__readArb_io_out_bits_setIdx;
        CData/*0:0*/ __Vtogcov__REG;
        CData/*0:0*/ __Vtogcov__r_0_valid;
        CData/*0:0*/ __Vtogcov__r_0_dirty;
        CData/*0:0*/ __Vtogcov__r_1_valid;
        CData/*0:0*/ __Vtogcov__r_1_dirty;
        CData/*0:0*/ __Vtogcov__r_2_valid;
        CData/*0:0*/ __Vtogcov__r_2_dirty;
        CData/*0:0*/ __Vtogcov__r_3_valid;
        CData/*0:0*/ __Vtogcov__r_3_dirty;
        CData/*0:0*/ __Vdly__REG;
        CData/*0:0*/ __Vdly__r_0_valid;
        CData/*0:0*/ __Vdly__r_0_dirty;
        CData/*0:0*/ __Vdly__r_1_valid;
        CData/*0:0*/ __Vdly__r_1_dirty;
        CData/*0:0*/ __Vdly__r_2_valid;
        CData/*0:0*/ __Vdly__r_2_dirty;
        CData/*0:0*/ __Vdly__r_3_valid;
        CData/*0:0*/ __Vdly__r_3_dirty;
        VL_OUT(__PVT__io_r_0_resp_data_0_tag,18,0);
        VL_OUT(__PVT__io_r_0_resp_data_1_tag,18,0);
        VL_OUT(__PVT__io_r_0_resp_data_2_tag,18,0);
        VL_OUT(__PVT__io_r_0_resp_data_3_tag,18,0);
        VL_IN(__PVT__io_w_req_bits_data_tag,18,0);
        IData/*18:0*/ __PVT__ram_io_r_resp_data_0_tag;
        IData/*18:0*/ __PVT__ram_io_r_resp_data_1_tag;
        IData/*18:0*/ __PVT__ram_io_r_resp_data_2_tag;
        IData/*18:0*/ __PVT__ram_io_r_resp_data_3_tag;
        IData/*18:0*/ __PVT__ram_io_w_req_bits_data_tag;
        IData/*18:0*/ __PVT__r_0_tag;
        IData/*18:0*/ __PVT__r_1_tag;
        IData/*18:0*/ __PVT__r_2_tag;
        IData/*18:0*/ __PVT__r_3_tag;
        IData/*18:0*/ __Vtogcov__io_r_0_resp_data_0_tag;
        IData/*18:0*/ __Vtogcov__io_r_0_resp_data_1_tag;
        IData/*18:0*/ __Vtogcov__io_r_0_resp_data_2_tag;
        IData/*18:0*/ __Vtogcov__io_r_0_resp_data_3_tag;
        IData/*18:0*/ __Vtogcov__io_w_req_bits_data_tag;
        IData/*18:0*/ __Vtogcov__ram_io_r_resp_data_0_tag;
        IData/*18:0*/ __Vtogcov__ram_io_r_resp_data_1_tag;
    };
    struct {
        IData/*18:0*/ __Vtogcov__ram_io_r_resp_data_2_tag;
        IData/*18:0*/ __Vtogcov__ram_io_r_resp_data_3_tag;
        IData/*18:0*/ __Vtogcov__ram_io_w_req_bits_data_tag;
        IData/*18:0*/ __Vtogcov__r_0_tag;
        IData/*18:0*/ __Vtogcov__r_1_tag;
        IData/*18:0*/ __Vtogcov__r_2_tag;
        IData/*18:0*/ __Vtogcov__r_3_tag;
        IData/*18:0*/ __Vdly__r_0_tag;
        IData/*18:0*/ __Vdly__r_1_tag;
        IData/*18:0*/ __Vdly__r_2_tag;
        IData/*18:0*/ __Vdly__r_3_tag;
    };

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_SRAMTemplateWithArbiter(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_SRAMTemplateWithArbiter();
    VL_UNCOPYABLE(Vnutshellcache_SRAMTemplateWithArbiter);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
