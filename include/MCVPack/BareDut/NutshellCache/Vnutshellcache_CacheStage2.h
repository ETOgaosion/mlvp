// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE_CACHESTAGE2_H_
#define VERILATED_VNUTSHELLCACHE_CACHESTAGE2_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache_CacheStage2 final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(__PVT__clock,0,0);
        VL_IN8(__PVT__reset,0,0);
        VL_OUT8(__PVT__io_in_ready,0,0);
        VL_IN8(__PVT__io_in_valid,0,0);
        VL_IN8(__PVT__io_in_bits_req_size,2,0);
        VL_IN8(__PVT__io_in_bits_req_cmd,3,0);
        VL_IN8(__PVT__io_in_bits_req_wmask,7,0);
        VL_IN8(__PVT__io_out_ready,0,0);
        VL_OUT8(__PVT__io_out_valid,0,0);
        VL_OUT8(__PVT__io_out_bits_req_size,2,0);
        VL_OUT8(__PVT__io_out_bits_req_cmd,3,0);
        VL_OUT8(__PVT__io_out_bits_req_wmask,7,0);
        VL_OUT8(__PVT__io_out_bits_metas_0_dirty,0,0);
        VL_OUT8(__PVT__io_out_bits_metas_1_dirty,0,0);
        VL_OUT8(__PVT__io_out_bits_metas_2_dirty,0,0);
        VL_OUT8(__PVT__io_out_bits_metas_3_dirty,0,0);
        VL_OUT8(__PVT__io_out_bits_hit,0,0);
        VL_OUT8(__PVT__io_out_bits_waymask,3,0);
        VL_OUT8(__PVT__io_out_bits_mmio,0,0);
        VL_OUT8(__PVT__io_out_bits_isForwardData,0,0);
        VL_OUT8(__PVT__io_out_bits_forwardData_waymask,3,0);
        VL_IN8(__PVT__io_metaReadResp_0_valid,0,0);
        VL_IN8(__PVT__io_metaReadResp_0_dirty,0,0);
        VL_IN8(__PVT__io_metaReadResp_1_valid,0,0);
        VL_IN8(__PVT__io_metaReadResp_1_dirty,0,0);
        VL_IN8(__PVT__io_metaReadResp_2_valid,0,0);
        VL_IN8(__PVT__io_metaReadResp_2_dirty,0,0);
        VL_IN8(__PVT__io_metaReadResp_3_valid,0,0);
        VL_IN8(__PVT__io_metaReadResp_3_dirty,0,0);
        VL_IN8(__PVT__io_metaWriteBus_req_valid,0,0);
        VL_IN8(__PVT__io_metaWriteBus_req_bits_setIdx,6,0);
        VL_IN8(__PVT__io_metaWriteBus_req_bits_data_dirty,0,0);
        VL_IN8(__PVT__io_metaWriteBus_req_bits_waymask,3,0);
        VL_IN8(__PVT__io_dataWriteBus_req_valid,0,0);
        VL_IN8(__PVT__io_dataWriteBus_req_bits_waymask,3,0);
        VL_OUT8(__PVT__victimWaymask,3,0);
        CData/*2:0*/ __PVT__addr_wordIndex;
        CData/*6:0*/ __PVT__addr_index;
        CData/*0:0*/ __PVT__isForwardMeta;
        CData/*0:0*/ __PVT__isForwardMetaReg;
        CData/*0:0*/ __PVT___GEN_0;
        CData/*0:0*/ __PVT___T;
        CData/*0:0*/ __PVT___T_1;
        CData/*0:0*/ __PVT___T_2;
        CData/*0:0*/ __PVT__forwardMetaReg_data_dirty;
        CData/*3:0*/ __PVT__forwardMetaReg_waymask;
        CData/*0:0*/ __PVT___GEN_5;
        CData/*3:0*/ __PVT___GEN_6;
        CData/*0:0*/ __PVT__pickForwardMeta;
        CData/*0:0*/ __PVT__forwardWaymask_0;
        CData/*0:0*/ __PVT__forwardWaymask_1;
        CData/*0:0*/ __PVT__forwardWaymask_2;
        CData/*0:0*/ __PVT__forwardWaymask_3;
        CData/*0:0*/ __PVT__metaWay_0_valid;
        CData/*0:0*/ __PVT__metaWay_1_valid;
        CData/*0:0*/ __PVT__metaWay_2_valid;
        CData/*0:0*/ __PVT__metaWay_3_valid;
        CData/*0:0*/ __PVT___hitVec_T_2;
        CData/*0:0*/ __PVT___hitVec_T_5;
        CData/*0:0*/ __PVT___hitVec_T_8;
        CData/*0:0*/ __PVT___hitVec_T_11;
        CData/*3:0*/ __PVT__hitVec;
        CData/*0:0*/ __PVT__victimWaymask_xor;
        CData/*0:0*/ __PVT___invalidVec_T;
    };
    struct {
        CData/*0:0*/ __PVT___invalidVec_T_1;
        CData/*0:0*/ __PVT___invalidVec_T_2;
        CData/*0:0*/ __PVT___invalidVec_T_3;
        CData/*3:0*/ __PVT__invalidVec;
        CData/*0:0*/ __PVT__hasInvalidWay;
        CData/*1:0*/ __PVT___refillInvalidWaymask_T_3;
        CData/*2:0*/ __PVT___refillInvalidWaymask_T_4;
        CData/*3:0*/ __PVT__refillInvalidWaymask;
        CData/*3:0*/ __PVT___waymask_T;
        CData/*3:0*/ __PVT__waymask;
        CData/*1:0*/ __PVT___T_7;
        CData/*1:0*/ __PVT___T_9;
        CData/*2:0*/ __PVT___T_11;
        CData/*0:0*/ __PVT___T_13;
        CData/*0:0*/ __PVT___T_16;
        CData/*0:0*/ __PVT___io_out_bits_mmio_T_2;
        CData/*0:0*/ __PVT___io_out_bits_mmio_T_5;
        CData/*0:0*/ __PVT___isForwardData_T_10;
        CData/*0:0*/ __PVT__isForwardData;
        CData/*0:0*/ __PVT__isForwardDataReg;
        CData/*0:0*/ __PVT___GEN_8;
        CData/*3:0*/ __PVT__forwardDataReg_waymask;
        CData/*0:0*/ __PVT___io_in_ready_T_1;
        CData/*0:0*/ __Vtogcov__clock;
        CData/*0:0*/ __Vtogcov__reset;
        CData/*0:0*/ __Vtogcov__io_in_ready;
        CData/*0:0*/ __Vtogcov__io_in_valid;
        CData/*2:0*/ __Vtogcov__io_in_bits_req_size;
        CData/*3:0*/ __Vtogcov__io_in_bits_req_cmd;
        CData/*7:0*/ __Vtogcov__io_in_bits_req_wmask;
        CData/*0:0*/ __Vtogcov__io_out_ready;
        CData/*0:0*/ __Vtogcov__io_out_valid;
        CData/*2:0*/ __Vtogcov__io_out_bits_req_size;
        CData/*3:0*/ __Vtogcov__io_out_bits_req_cmd;
        CData/*7:0*/ __Vtogcov__io_out_bits_req_wmask;
        CData/*0:0*/ __Vtogcov__io_out_bits_metas_0_dirty;
        CData/*0:0*/ __Vtogcov__io_out_bits_metas_1_dirty;
        CData/*0:0*/ __Vtogcov__io_out_bits_metas_2_dirty;
        CData/*0:0*/ __Vtogcov__io_out_bits_metas_3_dirty;
        CData/*0:0*/ __Vtogcov__io_out_bits_hit;
        CData/*3:0*/ __Vtogcov__io_out_bits_waymask;
        CData/*0:0*/ __Vtogcov__io_out_bits_mmio;
        CData/*0:0*/ __Vtogcov__io_out_bits_isForwardData;
        CData/*3:0*/ __Vtogcov__io_out_bits_forwardData_waymask;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_0_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_0_dirty;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_1_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_1_dirty;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_2_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_2_dirty;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_3_valid;
        CData/*0:0*/ __Vtogcov__io_metaReadResp_3_dirty;
        CData/*0:0*/ __Vtogcov__io_metaWriteBus_req_valid;
        CData/*6:0*/ __Vtogcov__io_metaWriteBus_req_bits_setIdx;
        CData/*0:0*/ __Vtogcov__io_metaWriteBus_req_bits_data_dirty;
        CData/*3:0*/ __Vtogcov__io_metaWriteBus_req_bits_waymask;
        CData/*0:0*/ __Vtogcov__io_dataWriteBus_req_valid;
        CData/*3:0*/ __Vtogcov__io_dataWriteBus_req_bits_waymask;
        CData/*3:0*/ __Vtogcov__victimWaymask;
        CData/*2:0*/ __Vtogcov__addr_wordIndex;
        CData/*6:0*/ __Vtogcov__addr_index;
        CData/*0:0*/ __Vtogcov__isForwardMeta;
        CData/*0:0*/ __Vtogcov__isForwardMetaReg;
        CData/*0:0*/ __Vtogcov__forwardMetaReg_data_dirty;
    };
    struct {
        CData/*3:0*/ __Vtogcov__forwardMetaReg_waymask;
        CData/*0:0*/ __Vtogcov__pickForwardMeta;
        CData/*0:0*/ __Vtogcov__forwardWaymask_0;
        CData/*0:0*/ __Vtogcov__forwardWaymask_1;
        CData/*0:0*/ __Vtogcov__forwardWaymask_2;
        CData/*0:0*/ __Vtogcov__forwardWaymask_3;
        CData/*0:0*/ __Vtogcov__metaWay_0_valid;
        CData/*0:0*/ __Vtogcov__metaWay_1_valid;
        CData/*0:0*/ __Vtogcov__metaWay_2_valid;
        CData/*0:0*/ __Vtogcov__metaWay_3_valid;
        CData/*3:0*/ __Vtogcov__hitVec;
        CData/*0:0*/ __Vtogcov__victimWaymask_xor;
        CData/*3:0*/ __Vtogcov__invalidVec;
        CData/*0:0*/ __Vtogcov__hasInvalidWay;
        CData/*3:0*/ __Vtogcov__refillInvalidWaymask;
        CData/*3:0*/ __Vtogcov__waymask;
        CData/*0:0*/ __Vtogcov__isForwardData;
        CData/*0:0*/ __Vtogcov__isForwardDataReg;
        CData/*3:0*/ __Vtogcov__forwardDataReg_waymask;
        CData/*0:0*/ __Vdly__isForwardMetaReg;
        CData/*0:0*/ __Vdly__forwardMetaReg_data_dirty;
        CData/*3:0*/ __Vdly__forwardMetaReg_waymask;
        CData/*0:0*/ __Vdly__isForwardDataReg;
        CData/*3:0*/ __Vdly__forwardDataReg_waymask;
        VL_IN16(__PVT__io_in_bits_req_user,15,0);
        VL_OUT16(__PVT__io_out_bits_req_user,15,0);
        VL_IN16(__PVT__io_dataWriteBus_req_bits_setIdx,9,0);
        SData/*9:0*/ __PVT___isForwardData_T_8;
        SData/*15:0*/ __Vtogcov__io_in_bits_req_user;
        SData/*15:0*/ __Vtogcov__io_out_bits_req_user;
        SData/*9:0*/ __Vtogcov__io_dataWriteBus_req_bits_setIdx;
        VL_IN(__PVT__io_in_bits_req_addr,31,0);
        VL_OUT(__PVT__io_out_bits_req_addr,31,0);
        VL_OUT(__PVT__io_out_bits_metas_0_tag,18,0);
        VL_OUT(__PVT__io_out_bits_metas_1_tag,18,0);
        VL_OUT(__PVT__io_out_bits_metas_2_tag,18,0);
        VL_OUT(__PVT__io_out_bits_metas_3_tag,18,0);
        VL_IN(__PVT__io_metaReadResp_0_tag,18,0);
        VL_IN(__PVT__io_metaReadResp_1_tag,18,0);
        VL_IN(__PVT__io_metaReadResp_2_tag,18,0);
        VL_IN(__PVT__io_metaReadResp_3_tag,18,0);
        VL_IN(__PVT__io_metaWriteBus_req_bits_data_tag,18,0);
        IData/*18:0*/ __PVT__addr_tag;
        IData/*18:0*/ __PVT__forwardMetaReg_data_tag;
        IData/*18:0*/ __PVT___GEN_3;
        IData/*18:0*/ __PVT__metaWay_0_tag;
        IData/*18:0*/ __PVT__metaWay_1_tag;
        IData/*18:0*/ __PVT__metaWay_2_tag;
        IData/*18:0*/ __PVT__metaWay_3_tag;
        IData/*31:0*/ __PVT___io_out_bits_mmio_T;
        IData/*31:0*/ __PVT___io_out_bits_mmio_T_3;
        IData/*31:0*/ __Vtogcov__io_in_bits_req_addr;
        IData/*31:0*/ __Vtogcov__io_out_bits_req_addr;
        IData/*18:0*/ __Vtogcov__io_out_bits_metas_0_tag;
        IData/*18:0*/ __Vtogcov__io_out_bits_metas_1_tag;
        IData/*18:0*/ __Vtogcov__io_out_bits_metas_2_tag;
        IData/*18:0*/ __Vtogcov__io_out_bits_metas_3_tag;
        IData/*18:0*/ __Vtogcov__io_metaReadResp_0_tag;
        IData/*18:0*/ __Vtogcov__io_metaReadResp_1_tag;
        IData/*18:0*/ __Vtogcov__io_metaReadResp_2_tag;
        IData/*18:0*/ __Vtogcov__io_metaReadResp_3_tag;
        IData/*18:0*/ __Vtogcov__io_metaWriteBus_req_bits_data_tag;
        IData/*18:0*/ __Vtogcov__addr_tag;
        IData/*18:0*/ __Vtogcov__forwardMetaReg_data_tag;
    };
    struct {
        IData/*18:0*/ __Vtogcov__metaWay_0_tag;
        IData/*18:0*/ __Vtogcov__metaWay_1_tag;
        IData/*18:0*/ __Vtogcov__metaWay_2_tag;
        IData/*18:0*/ __Vtogcov__metaWay_3_tag;
        IData/*18:0*/ __Vdly__forwardMetaReg_data_tag;
        VL_IN64(__PVT__io_in_bits_req_wdata,63,0);
        VL_OUT64(__PVT__io_out_bits_req_wdata,63,0);
        VL_OUT64(__PVT__io_out_bits_datas_0_data,63,0);
        VL_OUT64(__PVT__io_out_bits_datas_1_data,63,0);
        VL_OUT64(__PVT__io_out_bits_datas_2_data,63,0);
        VL_OUT64(__PVT__io_out_bits_datas_3_data,63,0);
        VL_OUT64(__PVT__io_out_bits_forwardData_data_data,63,0);
        VL_IN64(__PVT__io_dataReadResp_0_data,63,0);
        VL_IN64(__PVT__io_dataReadResp_1_data,63,0);
        VL_IN64(__PVT__io_dataReadResp_2_data,63,0);
        VL_IN64(__PVT__io_dataReadResp_3_data,63,0);
        VL_IN64(__PVT__io_dataWriteBus_req_bits_data_data,63,0);
        QData/*63:0*/ __PVT__victimWaymask_lfsr;
        QData/*63:0*/ __PVT___victimWaymask_lfsr_T_2;
        QData/*63:0*/ __PVT__forwardDataReg_data_data;
        QData/*63:0*/ __Vtogcov__io_in_bits_req_wdata;
        QData/*63:0*/ __Vtogcov__io_out_bits_req_wdata;
        QData/*63:0*/ __Vtogcov__io_out_bits_datas_0_data;
        QData/*63:0*/ __Vtogcov__io_out_bits_datas_1_data;
        QData/*63:0*/ __Vtogcov__io_out_bits_datas_2_data;
        QData/*63:0*/ __Vtogcov__io_out_bits_datas_3_data;
        QData/*63:0*/ __Vtogcov__io_out_bits_forwardData_data_data;
        QData/*63:0*/ __Vtogcov__io_dataReadResp_0_data;
        QData/*63:0*/ __Vtogcov__io_dataReadResp_1_data;
        QData/*63:0*/ __Vtogcov__io_dataReadResp_2_data;
        QData/*63:0*/ __Vtogcov__io_dataReadResp_3_data;
        QData/*63:0*/ __Vtogcov__io_dataWriteBus_req_bits_data_data;
        QData/*63:0*/ __Vtogcov__victimWaymask_lfsr;
        QData/*63:0*/ __Vtogcov__forwardDataReg_data_data;
        QData/*63:0*/ __Vdly__victimWaymask_lfsr;
        QData/*63:0*/ __Vdly__forwardDataReg_data_data;
    };

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache_CacheStage2(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache_CacheStage2();
    VL_UNCOPYABLE(Vnutshellcache_CacheStage2);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
