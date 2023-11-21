// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VNUTSHELLCACHE_H_
#define VERILATED_VNUTSHELLCACHE_H_  // guard

#include "verilated.h"
#include "verilated_cov.h"

class Vnutshellcache__Syms;
class Vnutshellcache___024root;
class Vnutshellcache_Cache;


// This class is the main interface to the Verilated model
class alignas(VL_CACHE_LINE_BYTES) Vnutshellcache VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vnutshellcache__Syms* const vlSymsp;

  public:

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clock,0,0);
    VL_IN8(&reset,0,0);
    VL_OUT8(&io_in_req_ready,0,0);
    VL_IN8(&io_in_req_valid,0,0);
    VL_IN8(&io_in_req_bits_size,2,0);
    VL_IN8(&io_in_req_bits_cmd,3,0);
    VL_IN8(&io_in_req_bits_wmask,7,0);
    VL_IN8(&io_in_resp_ready,0,0);
    VL_OUT8(&io_in_resp_valid,0,0);
    VL_OUT8(&io_in_resp_bits_cmd,3,0);
    VL_IN8(&io_flush,1,0);
    VL_IN8(&io_out_mem_req_ready,0,0);
    VL_OUT8(&io_out_mem_req_valid,0,0);
    VL_OUT8(&io_out_mem_req_bits_size,2,0);
    VL_OUT8(&io_out_mem_req_bits_cmd,3,0);
    VL_OUT8(&io_out_mem_req_bits_wmask,7,0);
    VL_OUT8(&io_out_mem_resp_ready,0,0);
    VL_IN8(&io_out_mem_resp_valid,0,0);
    VL_IN8(&io_out_mem_resp_bits_cmd,3,0);
    VL_OUT8(&io_out_coh_req_ready,0,0);
    VL_IN8(&io_out_coh_req_valid,0,0);
    VL_IN8(&io_out_coh_req_bits_size,2,0);
    VL_IN8(&io_out_coh_req_bits_cmd,3,0);
    VL_IN8(&io_out_coh_req_bits_wmask,7,0);
    VL_IN8(&io_out_coh_resp_ready,0,0);
    VL_OUT8(&io_out_coh_resp_valid,0,0);
    VL_OUT8(&io_out_coh_resp_bits_cmd,3,0);
    VL_IN8(&io_mmio_req_ready,0,0);
    VL_OUT8(&io_mmio_req_valid,0,0);
    VL_OUT8(&io_mmio_req_bits_size,2,0);
    VL_OUT8(&io_mmio_req_bits_cmd,3,0);
    VL_OUT8(&io_mmio_req_bits_wmask,7,0);
    VL_OUT8(&io_mmio_resp_ready,0,0);
    VL_IN8(&io_mmio_resp_valid,0,0);
    VL_IN8(&io_mmio_resp_bits_cmd,3,0);
    VL_OUT8(&io_empty,0,0);
    VL_OUT8(&victimWaymask,3,0);
    VL_IN16(&io_in_req_bits_user,15,0);
    VL_OUT16(&io_in_resp_bits_user,15,0);
    VL_IN(&io_in_req_bits_addr,31,0);
    VL_OUT(&io_out_mem_req_bits_addr,31,0);
    VL_IN(&io_out_coh_req_bits_addr,31,0);
    VL_OUT(&io_mmio_req_bits_addr,31,0);
    VL_IN64(&io_in_req_bits_wdata,63,0);
    VL_OUT64(&io_in_resp_bits_rdata,63,0);
    VL_OUT64(&io_out_mem_req_bits_wdata,63,0);
    VL_IN64(&io_out_mem_resp_bits_rdata,63,0);
    VL_IN64(&io_out_coh_req_bits_wdata,63,0);
    VL_OUT64(&io_out_coh_resp_bits_rdata,63,0);
    VL_OUT64(&io_mmio_req_bits_wdata,63,0);
    VL_IN64(&io_mmio_resp_bits_rdata,63,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.
    Vnutshellcache_Cache* const __PVT__Cache;

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vnutshellcache___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vnutshellcache(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vnutshellcache(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vnutshellcache();
  private:
    VL_UNCOPYABLE(Vnutshellcache);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    /// Prepare for cloning the model at the process level (e.g. fork in Linux)
    /// Release necessary resources. Called before cloning.
    void prepareClone() const;
    /// Re-init after cloning the model at the process level (e.g. fork in Linux)
    /// Re-allocate necessary resources. Called after cloning.
    void atClone() const;
};

#endif  // guard