// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_CACHESTAGE1_H_
#define VERILATED_VNUTSHELLCACHE_CACHESTAGE1_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_CacheStage1 final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_OUT8(__PVT__io_in_ready,0,0);
        VL_IN8(__PVT__io_in_valid,0,0);
        VL_IN8(__PVT__io_in_bits_size,2,0);
        VL_IN8(__PVT__io_in_bits_cmd,3,0);
        VL_IN8(__PVT__io_in_bits_wmask,7,0);
        VL_IN8(__PVT__io_out_ready,0,0);
        VL_OUT8(__PVT__io_out_valid,0,0);
        VL_OUT8(__PVT__io_out_bits_req_size,2,0);
        VL_OUT8(__PVT__io_out_bits_req_cmd,3,0);
        VL_OUT8(__PVT__io_out_bits_req_wmask,7,0);
        VL_IN8(__PVT__io_metaReadBus_req_ready,0,0);
        VL_OUT8(__PVT__io_metaReadBus_req_valid,0,0);
        VL_OUT8(__PVT__io_metaReadBus_req_bits_setIdx,6,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_0_valid,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_0_dirty,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_1_valid,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_1_dirty,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_2_valid,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_2_dirty,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_3_valid,0,0);
        VL_IN8(__PVT__io_metaReadBus_resp_data_3_dirty,0,0);
        VL_IN8(__PVT__io_dataReadBus_req_ready,0,0);
        VL_OUT8(__PVT__io_dataReadBus_req_valid,0,0);
        CData/*0:0*/ __PVT___io_in_ready_T_1;
        CData/*0:0*/ __Vtogcov__io_in_ready;
        CData/*0:0*/ __Vtogcov__io_in_valid;
        CData/*2:0*/ __Vtogcov__io_in_bits_size;
        CData/*3:0*/ __Vtogcov__io_in_bits_cmd;
        CData/*7:0*/ __Vtogcov__io_in_bits_wmask;
        CData/*0:0*/ __Vtogcov__io_out_ready;
        CData/*0:0*/ __Vtogcov__io_out_valid;
        CData/*2:0*/ __Vtogcov__io_out_bits_req_size;
        CData/*3:0*/ __Vtogcov__io_out_bits_req_cmd;
        CData/*7:0*/ __Vtogcov__io_out_bits_req_wmask;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_req_ready;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_req_valid;
        CData/*6:0*/ __Vtogcov__io_metaReadBus_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_0_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_0_dirty;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_1_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_1_dirty;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_2_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_2_dirty;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_3_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadBus_resp_data_3_dirty;
        CData/*0:0*/ __Vtogcov__io_dataReadBus_req_ready;
        CData/*0:0*/ __Vtogcov__io_dataReadBus_req_valid;
        VL_IN16(__PVT__io_in_bits_user,15,0);
        VL_OUT16(__PVT__io_out_bits_req_user,15,0);
        VL_OUT16(__PVT__io_dataReadBus_req_bits_setIdx,9,0);
        SData/*15:0*/ __Vtogcov__io_in_bits_user;
        SData/*15:0*/ __Vtogcov__io_out_bits_req_user;
        SData/*9:0*/ __Vtogcov__io_dataReadBus_req_bits_setIdx;
        VL_IN(__PVT__io_in_bits_addr,31,0);
        VL_OUT(__PVT__io_out_bits_req_addr,31,0);
        VL_IN(__PVT__io_metaReadBus_resp_data_0_tag,18,0);
        VL_IN(__PVT__io_metaReadBus_resp_data_1_tag,18,0);
        VL_IN(__PVT__io_metaReadBus_resp_data_2_tag,18,0);
        VL_IN(__PVT__io_metaReadBus_resp_data_3_tag,18,0);
        IData/*31:0*/ __Vtogcov__io_in_bits_addr;
        IData/*31:0*/ __Vtogcov__io_out_bits_req_addr;
        IData/*18:0*/ __Vtogcov__io_metaReadBus_resp_data_0_tag;
        IData/*18:0*/ __Vtogcov__io_metaReadBus_resp_data_1_tag;
        IData/*18:0*/ __Vtogcov__io_metaReadBus_resp_data_2_tag;
    };
    struct {
        IData/*18:0*/ __Vtogcov__io_metaReadBus_resp_data_3_tag;
        VL_IN64(__PVT__io_in_bits_wdata,63,0);
        VL_OUT64(__PVT__io_out_bits_req_wdata,63,0);
        VL_IN64(__PVT__io_dataReadBus_resp_data_0_data,63,0);
        VL_IN64(__PVT__io_dataReadBus_resp_data_1_data,63,0);
        VL_IN64(__PVT__io_dataReadBus_resp_data_2_data,63,0);
        VL_IN64(__PVT__io_dataReadBus_resp_data_3_data,63,0);
        QData/*63:0*/ __Vtogcov__io_in_bits_wdata;
        QData/*63:0*/ __Vtogcov__io_out_bits_req_wdata;
        QData/*63:0*/ __Vtogcov__io_dataReadBus_resp_data_0_data;
        QData/*63:0*/ __Vtogcov__io_dataReadBus_resp_data_1_data;
        QData/*63:0*/ __Vtogcov__io_dataReadBus_resp_data_2_data;
        QData/*63:0*/ __Vtogcov__io_dataReadBus_resp_data_3_data;
    };

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_CacheStage1(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_CacheStage1();
    VL_UNCOPYABLE(Vnutshellcache_CacheStage1);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
