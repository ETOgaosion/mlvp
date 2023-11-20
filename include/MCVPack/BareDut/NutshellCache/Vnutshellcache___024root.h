// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vnutshellcache.h for the primary calling header

#ifndef VERILATED_VNUTSHELLCACHE___024ROOT_H_
#define VERILATED_VNUTSHELLCACHE___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"
class Vnutshellcache_Cache;


class Vnutshellcache__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache___024root final : public VerilatedModule {
  public:
    // CELLS
    Vnutshellcache_Cache* __PVT__Cache;

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clock,0,0);
        VL_IN8(reset,0,0);
        VL_OUT8(io_in_req_ready,0,0);
        VL_IN8(io_in_req_valid,0,0);
        VL_IN8(io_in_req_bits_size,2,0);
        VL_IN8(io_in_req_bits_cmd,3,0);
        VL_IN8(io_in_req_bits_wmask,7,0);
        VL_IN8(io_in_resp_ready,0,0);
        VL_OUT8(io_in_resp_valid,0,0);
        VL_OUT8(io_in_resp_bits_cmd,3,0);
        VL_IN8(io_flush,1,0);
        VL_IN8(io_out_mem_req_ready,0,0);
        VL_OUT8(io_out_mem_req_valid,0,0);
        VL_OUT8(io_out_mem_req_bits_size,2,0);
        VL_OUT8(io_out_mem_req_bits_cmd,3,0);
        VL_OUT8(io_out_mem_req_bits_wmask,7,0);
        VL_OUT8(io_out_mem_resp_ready,0,0);
        VL_IN8(io_out_mem_resp_valid,0,0);
        VL_IN8(io_out_mem_resp_bits_cmd,3,0);
        VL_OUT8(io_out_coh_req_ready,0,0);
        VL_IN8(io_out_coh_req_valid,0,0);
        VL_IN8(io_out_coh_req_bits_size,2,0);
        VL_IN8(io_out_coh_req_bits_cmd,3,0);
        VL_IN8(io_out_coh_req_bits_wmask,7,0);
        VL_IN8(io_out_coh_resp_ready,0,0);
        VL_OUT8(io_out_coh_resp_valid,0,0);
        VL_OUT8(io_out_coh_resp_bits_cmd,3,0);
        VL_IN8(io_mmio_req_ready,0,0);
        VL_OUT8(io_mmio_req_valid,0,0);
        VL_OUT8(io_mmio_req_bits_size,2,0);
        VL_OUT8(io_mmio_req_bits_cmd,3,0);
        VL_OUT8(io_mmio_req_bits_wmask,7,0);
        VL_OUT8(io_mmio_resp_ready,0,0);
        VL_IN8(io_mmio_resp_valid,0,0);
        VL_IN8(io_mmio_resp_bits_cmd,3,0);
        VL_OUT8(io_empty,0,0);
        VL_OUT8(victimWaymask,3,0);
        CData/*0:0*/ __VstlContinue;
        CData/*0:0*/ __VicoContinue;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__clock__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__s2____PVT__clock__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__s3____PVT__clock__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__metaArray____PVT__clock__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__dataArray____PVT__clock__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__metaArray__ram____PVT__clock__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__Cache__dataArray__ram____PVT__clock__0;
        CData/*0:0*/ __VactContinue;
        CData/*0:0*/ __VnbaContinue;
        VL_IN16(io_in_req_bits_user,15,0);
        VL_OUT16(io_in_resp_bits_user,15,0);
        VL_IN(io_in_req_bits_addr,31,0);
        VL_OUT(io_out_mem_req_bits_addr,31,0);
        VL_IN(io_out_coh_req_bits_addr,31,0);
        VL_OUT(io_mmio_req_bits_addr,31,0);
        IData/*31:0*/ __VstlIterCount;
        IData/*31:0*/ __VicoIterCount;
        IData/*31:0*/ __VactIterCount;
        IData/*31:0*/ __VnbaIterCount;
        VL_IN64(io_in_req_bits_wdata,63,0);
        VL_OUT64(io_in_resp_bits_rdata,63,0);
        VL_OUT64(io_out_mem_req_bits_wdata,63,0);
        VL_IN64(io_out_mem_resp_bits_rdata,63,0);
        VL_IN64(io_out_coh_req_bits_wdata,63,0);
        VL_OUT64(io_out_coh_resp_bits_rdata,63,0);
    };
    struct {
        VL_OUT64(io_mmio_req_bits_wdata,63,0);
        VL_IN64(io_mmio_resp_bits_rdata,63,0);
    };
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<7> __VactTriggered;
    VlTriggerVec<7> __VpreTriggered;
    VlTriggerVec<7> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vnutshellcache__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vnutshellcache___024root(Vnutshellcache__Syms* symsp, const char* v__name);
    ~Vnutshellcache___024root();
    VL_UNCOPYABLE(Vnutshellcache___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
    void __vlCoverInsert(std::atomic<uint32_t>* countp, bool enable, const char* filenamep, int lineno, int column,
        const char* hierp, const char* pagep, const char* commentp, const char* linescovp);
};


#endif  // guard
