// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_SRAMTEMPLATE_1_H_
#define VERILATED_VNUTSHELLCACHE_SRAMTEMPLATE_1_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_SRAMTemplate_1 final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(__PVT__clock,0,0);
        VL_OUT8(__PVT__io_r_req_ready,0,0);
        VL_IN8(__PVT__io_r_req_valid,0,0);
        VL_IN8(__PVT__io_w_req_valid,0,0);
        VL_IN8(__PVT__io_w_req_bits_waymask,3,0);
        CData/*0:0*/ __PVT__array_0_rdata_MPORT_en;
        CData/*0:0*/ __PVT__array_0_MPORT_mask;
        CData/*0:0*/ __PVT__array_0_MPORT_en;
        CData/*0:0*/ __PVT__array_0_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __PVT__array_1_rdata_MPORT_en;
        CData/*0:0*/ __PVT__array_1_MPORT_mask;
        CData/*0:0*/ __PVT__array_1_MPORT_en;
        CData/*0:0*/ __PVT__array_1_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __PVT__array_2_rdata_MPORT_en;
        CData/*0:0*/ __PVT__array_2_MPORT_mask;
        CData/*0:0*/ __PVT__array_2_MPORT_en;
        CData/*0:0*/ __PVT__array_2_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __PVT__array_3_rdata_MPORT_en;
        CData/*0:0*/ __PVT__array_3_MPORT_mask;
        CData/*0:0*/ __PVT__array_3_MPORT_en;
        CData/*0:0*/ __PVT__array_3_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __PVT___realRen_T;
        CData/*0:0*/ __Vtogcov__clock;
        CData/*0:0*/ __Vtogcov__io_r_req_ready;
        CData/*0:0*/ __Vtogcov__io_r_req_valid;
        CData/*0:0*/ __Vtogcov__io_w_req_valid;
        CData/*3:0*/ __Vtogcov__io_w_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__array_0_rdata_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_0_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_0_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_0_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vtogcov__array_1_rdata_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_1_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_1_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_1_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vtogcov__array_2_rdata_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_2_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_2_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_2_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vtogcov__array_3_rdata_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_3_MPORT_mask;
        CData/*0:0*/ __Vtogcov__array_3_MPORT_en;
        CData/*0:0*/ __Vtogcov__array_3_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vdlyvset__array_0__v0;
        CData/*0:0*/ __Vdly__array_0_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vdlyvset__array_1__v0;
        CData/*0:0*/ __Vdly__array_1_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vdlyvset__array_2__v0;
        CData/*0:0*/ __Vdly__array_2_rdata_MPORT_en_pipe_0;
        CData/*0:0*/ __Vdlyvset__array_3__v0;
        CData/*0:0*/ __Vdly__array_3_rdata_MPORT_en_pipe_0;
        VL_IN16(__PVT__io_r_req_bits_setIdx,9,0);
        VL_IN16(__PVT__io_w_req_bits_setIdx,9,0);
        SData/*9:0*/ __PVT__array_0_rdata_MPORT_addr;
        SData/*9:0*/ __PVT__array_0_MPORT_addr;
        SData/*9:0*/ __PVT__array_0_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __PVT__array_1_rdata_MPORT_addr;
        SData/*9:0*/ __PVT__array_1_MPORT_addr;
        SData/*9:0*/ __PVT__array_1_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __PVT__array_2_rdata_MPORT_addr;
        SData/*9:0*/ __PVT__array_2_MPORT_addr;
        SData/*9:0*/ __PVT__array_2_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __PVT__array_3_rdata_MPORT_addr;
        SData/*9:0*/ __PVT__array_3_MPORT_addr;
    };
    struct {
        SData/*9:0*/ __PVT__array_3_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vtogcov__io_r_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__io_w_req_bits_setIdx;
        SData/*9:0*/ __Vtogcov__array_0_rdata_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_0_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_0_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vtogcov__array_1_rdata_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_1_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_1_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vtogcov__array_2_rdata_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_2_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_2_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vtogcov__array_3_rdata_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_3_MPORT_addr;
        SData/*9:0*/ __Vtogcov__array_3_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vdlyvdim0__array_0__v0;
        SData/*9:0*/ __Vdly__array_0_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vdlyvdim0__array_1__v0;
        SData/*9:0*/ __Vdly__array_1_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vdlyvdim0__array_2__v0;
        SData/*9:0*/ __Vdly__array_2_rdata_MPORT_addr_pipe_0;
        SData/*9:0*/ __Vdlyvdim0__array_3__v0;
        SData/*9:0*/ __Vdly__array_3_rdata_MPORT_addr_pipe_0;
        VL_OUT64(__PVT__io_r_resp_data_0_data,63,0);
        VL_OUT64(__PVT__io_r_resp_data_1_data,63,0);
        VL_OUT64(__PVT__io_r_resp_data_2_data,63,0);
        VL_OUT64(__PVT__io_r_resp_data_3_data,63,0);
        VL_IN64(__PVT__io_w_req_bits_data_data,63,0);
        QData/*63:0*/ __PVT__array_0_rdata_MPORT_data;
        QData/*63:0*/ __PVT__array_0_MPORT_data;
        QData/*63:0*/ __PVT__array_1_rdata_MPORT_data;
        QData/*63:0*/ __PVT__array_1_MPORT_data;
        QData/*63:0*/ __PVT__array_2_rdata_MPORT_data;
        QData/*63:0*/ __PVT__array_2_MPORT_data;
        QData/*63:0*/ __PVT__array_3_rdata_MPORT_data;
        QData/*63:0*/ __PVT__array_3_MPORT_data;
        QData/*63:0*/ __Vtogcov__io_r_resp_data_0_data;
        QData/*63:0*/ __Vtogcov__io_r_resp_data_1_data;
        QData/*63:0*/ __Vtogcov__io_r_resp_data_2_data;
        QData/*63:0*/ __Vtogcov__io_r_resp_data_3_data;
        QData/*63:0*/ __Vtogcov__io_w_req_bits_data_data;
        QData/*63:0*/ __Vtogcov__array_0_rdata_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_0_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_1_rdata_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_1_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_2_rdata_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_2_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_3_rdata_MPORT_data;
        QData/*63:0*/ __Vtogcov__array_3_MPORT_data;
        QData/*63:0*/ __Vdlyvval__array_0__v0;
        QData/*63:0*/ __Vdlyvval__array_1__v0;
        QData/*63:0*/ __Vdlyvval__array_2__v0;
        QData/*63:0*/ __Vdlyvval__array_3__v0;
        VlUnpacked<QData/*63:0*/, 1024> __PVT__array_0;
        VlUnpacked<QData/*63:0*/, 1024> __PVT__array_1;
        VlUnpacked<QData/*63:0*/, 1024> __PVT__array_2;
        VlUnpacked<QData/*63:0*/, 1024> __PVT__array_3;
    };

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_SRAMTemplate_1(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_SRAMTemplate_1();
    VL_UNCOPYABLE(Vnutshellcache_SRAMTemplate_1);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
