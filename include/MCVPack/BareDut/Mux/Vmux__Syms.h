// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VMUX__SYMS_H_
#define VERILATED_VMUX__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vmux.h"

// INCLUDE MODULE CLASSES
#include "Vmux___024root.h"
#include "Vmux_mux.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vmux__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vmux* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vmux___024root                 TOP;
    Vmux_mux                       TOP__mux;

    // COVERAGE
    std::atomic<uint32_t> __Vcoverage[7];

    // CONSTRUCTORS
    Vmux__Syms(VerilatedContext* contextp, const char* namep, Vmux* modelp);
    ~Vmux__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
