// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_SRAMTEMPLATE_H_
#define VERILATED_VNUTSHELLCACHE_SRAMTEMPLATE_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_SRAMTemplate final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(__PVT__clock,0,0);
        VL_IN8(__PVT__reset,0,0);
        VL_OUT8(__PVT__io_r_req_ready,0,0);
        VL_IN8(__PVT__io_r_req_valid,0,0);
        VL_IN8(__PVT__io_r_req_bits_setIdx,6,0);
        VL_OUT8(__PVT__io_r_resp_data_0_valid,0,0);
        VL_OUT8(__PVT__io_r_resp_data_0_dirty,0,0);
        VL_OUT8(__PVT__io_r_resp_data_1_valid,0,0);
        VL_OUT8(__PVT__io_r_resp_data_1_dirty,0,0);
        VL_OUT8(__PVT__io_r_resp_data_2_valid,0,0);
        VL_OUT8(__PVT__io_r_resp_data_2_dirty,0,0);
        VL_OUT8(__PVT__io_r_resp_data_3_valid,0,0);
        VL_OUT8(__PVT__io_r_resp_data_3_dirty,0,0);
        VL_IN8(__PVT__io_w_req_valid,0,0);
        VL_IN8(__PVT__io_w_req_bits_setIdx,6,0);
        VL_IN8(__PVT__io_w_req_bits_data_dirty,0,0);
        VL_IN8(__PVT__io_w_req_bits_waymask,3,0);
        CData/*0:0*/ __PVT__array_0_rdata_MPORT_en;
        CData/*6:0*/ __PVT__array_0_rdata_MPORT_addr;
        CData/*6:0*/ __PVT__array_0_MPORT_addr;
        CData/*0:0*/ __PVT__array_0_MPORT_mask;
        CData/*0:0*/ __PVT__array_0_MPORT_en;
        CData/*0:0*/ __PVT__array_0_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __PVT__array_0_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __PVT__array_1_rdata_MPORT_en;
        CData/*6:0*/ __PVT__array_1_rdata_MPORT_addr;
        CData/*6:0*/ __PVT__array_1_MPORT_addr;
        CData/*0:0*/ __PVT__array_1_MPORT_mask;
        CData/*0:0*/ __PVT__array_1_MPORT_en;
        CData/*0:0*/ __PVT__array_1_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __PVT__array_1_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __PVT__array_2_rdata_MPORT_en;
        CData/*6:0*/ __PVT__array_2_rdata_MPORT_addr;
        CData/*6:0*/ __PVT__array_2_MPORT_addr;
        CData/*0:0*/ __PVT__array_2_MPORT_mask;
        CData/*0:0*/ __PVT__array_2_MPORT_en;
        CData/*0:0*/ __PVT__array_2_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __PVT__array_2_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __PVT__array_3_rdata_MPORT_en;
        CData/*6:0*/ __PVT__array_3_rdata_MPORT_addr;
        CData/*6:0*/ __PVT__array_3_MPORT_addr;
        CData/*0:0*/ __PVT__array_3_MPORT_mask;
        CData/*0:0*/ __PVT__array_3_MPORT_en;
        CData/*0:0*/ __PVT__array_3_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __PVT__array_3_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __PVT__resetState;
        CData/*6:0*/ __PVT__resetSet;
        CData/*0:0*/ __PVT__wrap_wrap;
        CData/*6:0*/ __PVT___wrap_value_T_1;
        CData/*0:0*/ __PVT__resetFinish;
        CData/*0:0*/ __PVT___GEN_2;
        CData/*0:0*/ __PVT__wen;
        CData/*0:0*/ __PVT___realRen_T;
        CData/*3:0*/ __PVT__waymask;
        CData/*0:0*/ __Vtogcov__clock;
        CData/*0:0*/ __Vtogcov__reset;
        CData/*0:0*/ __Vtogcov__io_r_req_ready;
        CData/*0:0*/ __Vtogcov__io_r_req_valid;
        CData/*6:0*/ __Vtogcov__io_r_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_0_valid;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_0_dirty;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_1_valid;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_1_dirty;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_2_valid;
    };
    struct {
        CData/*0:0*/ __Vtogcov__io_r_resp_data_2_dirty;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_3_valid;
        CData/*0:0*/ __Vtogcov__io_r_resp_data_3_dirty;
        CData/*0:0*/ __Vtogcov__io_w_req_valid;
        CData/*6:0*/ __Vtogcov__io_w_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__io_w_req_bits_data_dirty;
        CData/*3:0*/ __Vtogcov__io_w_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__array_0_rdata_MPORT_en;
        CData/*6:0*/ __Vtogcov__array_0_rdata_MPORT_addr;
        CData/*6:0*/ __Vtogcov__array_0_MPORT_addr;
        CData/*0:0*/ __Vtogcov__array_0_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_0_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_0_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vtogcov__array_0_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __Vtogcov__array_1_rdata_MPORT_en;
        CData/*6:0*/ __Vtogcov__array_1_rdata_MPORT_addr;
        CData/*6:0*/ __Vtogcov__array_1_MPORT_addr;
        CData/*0:0*/ __Vtogcov__array_1_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_1_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_1_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vtogcov__array_1_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __Vtogcov__array_2_rdata_MPORT_en;
        CData/*6:0*/ __Vtogcov__array_2_rdata_MPORT_addr;
        CData/*6:0*/ __Vtogcov__array_2_MPORT_addr;
        CData/*0:0*/ __Vtogcov__array_2_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_2_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_2_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vtogcov__array_2_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __Vtogcov__array_3_rdata_MPORT_en;
        CData/*6:0*/ __Vtogcov__array_3_rdata_MPORT_addr;
        CData/*6:0*/ __Vtogcov__array_3_MPORT_addr;
        CData/*0:0*/ __Vtogcov__array_3_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_3_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_3_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vtogcov__array_3_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __Vtogcov__resetState;
        CData/*6:0*/ __Vtogcov__resetSet;
        CData/*0:0*/ __Vtogcov__wrap_wrap;
        CData/*0:0*/ __Vtogcov__resetFinish;
        CData/*0:0*/ __Vtogcov__wen;
        CData/*3:0*/ __Vtogcov__waymask;
        CData/*6:0*/ __Vdlyvdim0__array_0__v0;
        CData/*0:0*/ __Vdlyvset__array_0__v0;
        CData/*0:0*/ __Vdly__array_0_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vdly__array_0_rdata_MPORT_addr_pipe_0;
        CData/*6:0*/ __Vdlyvdim0__array_1__v0;
        CData/*0:0*/ __Vdlyvset__array_1__v0;
        CData/*0:0*/ __Vdly__array_1_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vdly__array_1_rdata_MPORT_addr_pipe_0;
        CData/*6:0*/ __Vdlyvdim0__array_2__v0;
        CData/*0:0*/ __Vdlyvset__array_2__v0;
        CData/*0:0*/ __Vdly__array_2_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vdly__array_2_rdata_MPORT_addr_pipe_0;
        CData/*6:0*/ __Vdlyvdim0__array_3__v0;
        CData/*0:0*/ __Vdlyvset__array_3__v0;
        CData/*0:0*/ __Vdly__array_3_rdata_MPORT_en_pipe_0;
        CData/*6:0*/ __Vdly__array_3_rdata_MPORT_addr_pipe_0;
        CData/*0:0*/ __Vdly__resetState;
        CData/*6:0*/ __Vdly__resetSet;
        VL_OUT(__PVT__io_r_resp_data_0_tag,18,0);
        VL_OUT(__PVT__io_r_resp_data_1_tag,18,0);
        VL_OUT(__PVT__io_r_resp_data_2_tag,18,0);
        VL_OUT(__PVT__io_r_resp_data_3_tag,18,0);
        VL_IN(__PVT__io_w_req_bits_data_tag,18,0);
    };
    struct {
        IData/*20:0*/ __PVT__array_0_rdata_MPORT_data;
        IData/*20:0*/ __PVT__array_0_MPORT_data;
        IData/*20:0*/ __PVT__array_1_rdata_MPORT_data;
        IData/*20:0*/ __PVT__array_1_MPORT_data;
        IData/*20:0*/ __PVT__array_2_rdata_MPORT_data;
        IData/*20:0*/ __PVT__array_2_MPORT_data;
        IData/*20:0*/ __PVT__array_3_rdata_MPORT_data;
        IData/*20:0*/ __PVT__array_3_MPORT_data;
        IData/*20:0*/ __PVT___wdataword_T;
        IData/*20:0*/ __PVT___rdata_WIRE_1;
        IData/*20:0*/ __PVT___rdata_WIRE_2;
        IData/*20:0*/ __PVT___rdata_WIRE_3;
        IData/*20:0*/ __PVT___rdata_WIRE_4;
        IData/*18:0*/ __Vtogcov__io_r_resp_data_0_tag;
        IData/*18:0*/ __Vtogcov__io_r_resp_data_1_tag;
        IData/*18:0*/ __Vtogcov__io_r_resp_data_2_tag;
        IData/*18:0*/ __Vtogcov__io_r_resp_data_3_tag;
        IData/*18:0*/ __Vtogcov__io_w_req_bits_data_tag;
        IData/*20:0*/ __Vtogcov__array_0_rdata_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_0_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_1_rdata_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_1_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_2_rdata_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_2_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_3_rdata_MPORT_data;
        IData/*20:0*/ __Vtogcov__array_3_MPORT_data;
        IData/*20:0*/ __Vdlyvval__array_0__v0;
        IData/*20:0*/ __Vdlyvval__array_1__v0;
        IData/*20:0*/ __Vdlyvval__array_2__v0;
        IData/*20:0*/ __Vdlyvval__array_3__v0;
        VlUnpacked<IData/*20:0*/, 128> __PVT__array_0;
        VlUnpacked<IData/*20:0*/, 128> __PVT__array_1;
        VlUnpacked<IData/*20:0*/, 128> __PVT__array_2;
        VlUnpacked<IData/*20:0*/, 128> __PVT__array_3;
    };

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_SRAMTemplate(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_SRAMTemplate();
    VL_UNCOPYABLE(Vnutshellcache_SRAMTemplate);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
