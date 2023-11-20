// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VNUTSHELLCACHE__SYMS_H_
#define VERILATED_VNUTSHELLCACHE__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vnutshellcache.h"

// INCLUDE MODULE CLASSES
#include "Vnutshellcache___024root.h"
#include "Vnutshellcache_Cache.h"
#include "Vnutshellcache_CacheStage1.h"
#include "Vnutshellcache_CacheStage2.h"
#include "Vnutshellcache_CacheStage3.h"
#include "Vnutshellcache_SRAMTemplateWithArbiter.h"
#include "Vnutshellcache_SRAMTemplateWithArbiter_1.h"
#include "Vnutshellcache_Arbiter_4.h"
#include "Vnutshellcache_Arbiter.h"
#include "Vnutshellcache_Arbiter_1.h"
#include "Vnutshellcache_SRAMTemplate.h"
#include "Vnutshellcache_Arbiter_2.h"
#include "Vnutshellcache_SRAMTemplate_1.h"
#include "Vnutshellcache_Arbiter_3.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vnutshellcache__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vnutshellcache* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vnutshellcache___024root       TOP;
    Vnutshellcache_Cache           TOP__Cache;
    Vnutshellcache_Arbiter_4       TOP__Cache__arb;
    Vnutshellcache_SRAMTemplateWithArbiter_1 TOP__Cache__dataArray;
    Vnutshellcache_SRAMTemplate_1  TOP__Cache__dataArray__ram;
    Vnutshellcache_Arbiter_3       TOP__Cache__dataArray__readArb;
    Vnutshellcache_SRAMTemplateWithArbiter TOP__Cache__metaArray;
    Vnutshellcache_SRAMTemplate    TOP__Cache__metaArray__ram;
    Vnutshellcache_Arbiter_2       TOP__Cache__metaArray__readArb;
    Vnutshellcache_CacheStage1     TOP__Cache__s1;
    Vnutshellcache_CacheStage2     TOP__Cache__s2;
    Vnutshellcache_CacheStage3     TOP__Cache__s3;
    Vnutshellcache_Arbiter_1       TOP__Cache__s3__dataWriteArb;
    Vnutshellcache_Arbiter         TOP__Cache__s3__metaWriteArb;

    // COVERAGE
    std::atomic<uint32_t> __Vcoverage[14432];

    // SCOPE NAMES
    VerilatedScope __Vscope_Cache__s2;
    VerilatedScope __Vscope_Cache__s3;

    // CONSTRUCTORS
    Vnutshellcache__Syms(VerilatedContext* contextp, const char* namep, Vnutshellcache* modelp);
    ~Vnutshellcache__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
