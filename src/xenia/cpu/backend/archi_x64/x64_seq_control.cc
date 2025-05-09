
#include "xenia/cpu/backend/x64/x64_sequences.h"

#include <algorithm>
#include <cstring>

#include "xenia/cpu/backend/x64/x64_op.h"

namespace xe {
namespace cpu {
namespace backend {
namespace x64 {

volatile int anchor_control = 0;

// ============================================================================
// OPCODE_DEBUG_BREAK
// ============================================================================
struct DEBUG_BREAK : Sequence<DEBUG_BREAK, I<OPCODE_DEBUG_BREAK, VoidOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) { e.DebugBreak(); }
};
EMITTER_OPCODE_TABLE(OPCODE_DEBUG_BREAK, DEBUG_BREAK);

// ============================================================================
// OPCODE_DEBUG_BREAK_TRUE
// ============================================================================
struct DEBUG_BREAK_TRUE_I8
    : Sequence<DEBUG_BREAK_TRUE_I8, I<OPCODE_DEBUG_BREAK_TRUE, VoidOp, I8Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.DebugBreak();
    e.L(skip);
  }
};
struct DEBUG_BREAK_TRUE_I16
    : Sequence<DEBUG_BREAK_TRUE_I16,
               I<OPCODE_DEBUG_BREAK_TRUE, VoidOp, I16Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.DebugBreak();
    e.L(skip);
  }
};
struct DEBUG_BREAK_TRUE_I32
    : Sequence<DEBUG_BREAK_TRUE_I32,
               I<OPCODE_DEBUG_BREAK_TRUE, VoidOp, I32Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.DebugBreak();
    e.L(skip);
  }
};
struct DEBUG_BREAK_TRUE_I64
    : Sequence<DEBUG_BREAK_TRUE_I64,
               I<OPCODE_DEBUG_BREAK_TRUE, VoidOp, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.DebugBreak();
    e.L(skip);
  }
};
struct DEBUG_BREAK_TRUE_F32
    : Sequence<DEBUG_BREAK_TRUE_F32,
               I<OPCODE_DEBUG_BREAK_TRUE, VoidOp, F32Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.DebugBreak();
    e.L(skip);
  }
};
struct DEBUG_BREAK_TRUE_F64
    : Sequence<DEBUG_BREAK_TRUE_F64,
               I<OPCODE_DEBUG_BREAK_TRUE, VoidOp, F64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.DebugBreak();
    e.L(skip);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_DEBUG_BREAK_TRUE, DEBUG_BREAK_TRUE_I8,
                     DEBUG_BREAK_TRUE_I16, DEBUG_BREAK_TRUE_I32,
                     DEBUG_BREAK_TRUE_I64, DEBUG_BREAK_TRUE_F32,
                     DEBUG_BREAK_TRUE_F64);

// ============================================================================
// OPCODE_TRAP
// ============================================================================
struct TRAP : Sequence<TRAP, I<OPCODE_TRAP, VoidOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.Trap(i.instr->flags);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_TRAP, TRAP);

// ============================================================================
// OPCODE_TRAP_TRUE
// ============================================================================
struct TRAP_TRUE_I8
    : Sequence<TRAP_TRUE_I8, I<OPCODE_TRAP_TRUE, VoidOp, I8Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Trap(i.instr->flags);
    e.L(skip);
  }
};
struct TRAP_TRUE_I16
    : Sequence<TRAP_TRUE_I16, I<OPCODE_TRAP_TRUE, VoidOp, I16Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Trap(i.instr->flags);
    e.L(skip);
  }
};
struct TRAP_TRUE_I32
    : Sequence<TRAP_TRUE_I32, I<OPCODE_TRAP_TRUE, VoidOp, I32Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Trap(i.instr->flags);
    e.L(skip);
  }
};
struct TRAP_TRUE_I64
    : Sequence<TRAP_TRUE_I64, I<OPCODE_TRAP_TRUE, VoidOp, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Trap(i.instr->flags);
    e.L(skip);
  }
};
struct TRAP_TRUE_F32
    : Sequence<TRAP_TRUE_F32, I<OPCODE_TRAP_TRUE, VoidOp, F32Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Trap(i.instr->flags);
    e.L(skip);
  }
};
struct TRAP_TRUE_F64
    : Sequence<TRAP_TRUE_F64, I<OPCODE_TRAP_TRUE, VoidOp, F64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Trap(i.instr->flags);
    e.L(skip);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_TRAP_TRUE, TRAP_TRUE_I8, TRAP_TRUE_I16,
                     TRAP_TRUE_I32, TRAP_TRUE_I64, TRAP_TRUE_F32,
                     TRAP_TRUE_F64);

// ============================================================================
// OPCODE_CALL
// ============================================================================
struct CALL : Sequence<CALL, I<OPCODE_CALL, VoidOp, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src1.value->is_guest());
    e.Call(i.instr, static_cast<GuestFunction*>(i.src1.value));
  }
};
EMITTER_OPCODE_TABLE(OPCODE_CALL, CALL);

// ============================================================================
// OPCODE_CALL_TRUE
// ============================================================================
struct CALL_TRUE_I8
    : Sequence<CALL_TRUE_I8, I<OPCODE_CALL_TRUE, VoidOp, I8Op, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src2.value->is_guest());
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Call(i.instr, static_cast<GuestFunction*>(i.src2.value));
    e.L(skip);
  }
};
struct CALL_TRUE_I16
    : Sequence<CALL_TRUE_I16, I<OPCODE_CALL_TRUE, VoidOp, I16Op, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src2.value->is_guest());
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Call(i.instr, static_cast<GuestFunction*>(i.src2.value));
    e.L(skip);
  }
};
struct CALL_TRUE_I32
    : Sequence<CALL_TRUE_I32, I<OPCODE_CALL_TRUE, VoidOp, I32Op, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src2.value->is_guest());
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Call(i.instr, static_cast<GuestFunction*>(i.src2.value));
    e.L(skip);
  }
};
struct CALL_TRUE_I64
    : Sequence<CALL_TRUE_I64, I<OPCODE_CALL_TRUE, VoidOp, I64Op, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src2.value->is_guest());
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Call(i.instr, static_cast<GuestFunction*>(i.src2.value));
    e.L(skip);
  }
};
struct CALL_TRUE_F32
    : Sequence<CALL_TRUE_F32, I<OPCODE_CALL_TRUE, VoidOp, F32Op, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src2.value->is_guest());
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Call(i.instr, static_cast<GuestFunction*>(i.src2.value));
    e.L(skip);
  }
};
struct CALL_TRUE_F64
    : Sequence<CALL_TRUE_F64, I<OPCODE_CALL_TRUE, VoidOp, F64Op, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    assert_true(i.src2.value->is_guest());
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip);
    e.Call(i.instr, static_cast<GuestFunction*>(i.src2.value));
    e.L(skip);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_CALL_TRUE, CALL_TRUE_I8, CALL_TRUE_I16,
                     CALL_TRUE_I32, CALL_TRUE_I64, CALL_TRUE_F32,
                     CALL_TRUE_F64);

// ============================================================================
// OPCODE_CALL_INDIRECT
// ============================================================================
struct CALL_INDIRECT
    : Sequence<CALL_INDIRECT, I<OPCODE_CALL_INDIRECT, VoidOp, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.CallIndirect(i.instr, i.src1);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_CALL_INDIRECT, CALL_INDIRECT);

// ============================================================================
// OPCODE_CALL_INDIRECT_TRUE
// ============================================================================
struct CALL_INDIRECT_TRUE_I8
    : Sequence<CALL_INDIRECT_TRUE_I8,
               I<OPCODE_CALL_INDIRECT_TRUE, VoidOp, I8Op, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip, CodeGenerator::T_NEAR);
    e.CallIndirect(i.instr, i.src2);
    e.L(skip);
  }
};
struct CALL_INDIRECT_TRUE_I16
    : Sequence<CALL_INDIRECT_TRUE_I16,
               I<OPCODE_CALL_INDIRECT_TRUE, VoidOp, I16Op, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip, CodeGenerator::T_NEAR);
    e.CallIndirect(i.instr, i.src2);
    e.L(skip);
  }
};
struct CALL_INDIRECT_TRUE_I32
    : Sequence<CALL_INDIRECT_TRUE_I32,
               I<OPCODE_CALL_INDIRECT_TRUE, VoidOp, I32Op, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip, CodeGenerator::T_NEAR);
    e.CallIndirect(i.instr, i.src2);
    e.L(skip);
  }
};
struct CALL_INDIRECT_TRUE_I64
    : Sequence<CALL_INDIRECT_TRUE_I64,
               I<OPCODE_CALL_INDIRECT_TRUE, VoidOp, I64Op, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip, CodeGenerator::T_NEAR);
    e.CallIndirect(i.instr, i.src2);
    e.L(skip);
  }
};
struct CALL_INDIRECT_TRUE_F32
    : Sequence<CALL_INDIRECT_TRUE_F32,
               I<OPCODE_CALL_INDIRECT_TRUE, VoidOp, F32Op, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip, CodeGenerator::T_NEAR);
    e.CallIndirect(i.instr, i.src2);
    e.L(skip);
  }
};
struct CALL_INDIRECT_TRUE_F64
    : Sequence<CALL_INDIRECT_TRUE_F64,
               I<OPCODE_CALL_INDIRECT_TRUE, VoidOp, F64Op, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    Xbyak::Label skip;
    e.jz(skip, CodeGenerator::T_NEAR);
    e.CallIndirect(i.instr, i.src2);
    e.L(skip);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_CALL_INDIRECT_TRUE, CALL_INDIRECT_TRUE_I8,
                     CALL_INDIRECT_TRUE_I16, CALL_INDIRECT_TRUE_I32,
                     CALL_INDIRECT_TRUE_I64, CALL_INDIRECT_TRUE_F32,
                     CALL_INDIRECT_TRUE_F64);

// ============================================================================
// OPCODE_CALL_EXTERN
// ============================================================================
struct CALL_EXTERN
    : Sequence<CALL_EXTERN, I<OPCODE_CALL_EXTERN, VoidOp, SymbolOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.CallExtern(i.instr, i.src1.value);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_CALL_EXTERN, CALL_EXTERN);

// ============================================================================
// OPCODE_RETURN
// ============================================================================
struct RETURN : Sequence<RETURN, I<OPCODE_RETURN, VoidOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    // If this is the last instruction in the last block, just let us
    // fall through.
    if (i.instr->next || i.instr->block->next) {
      e.jmp(e.epilog_label(), CodeGenerator::T_NEAR);
    }
  }
};
EMITTER_OPCODE_TABLE(OPCODE_RETURN, RETURN);

// ============================================================================
// OPCODE_RETURN_TRUE
// ============================================================================
struct RETURN_TRUE_I8
    : Sequence<RETURN_TRUE_I8, I<OPCODE_RETURN_TRUE, VoidOp, I8Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(e.epilog_label(), CodeGenerator::T_NEAR);
  }
};
struct RETURN_TRUE_I16
    : Sequence<RETURN_TRUE_I16, I<OPCODE_RETURN_TRUE, VoidOp, I16Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(e.epilog_label(), CodeGenerator::T_NEAR);
  }
};
struct RETURN_TRUE_I32
    : Sequence<RETURN_TRUE_I32, I<OPCODE_RETURN_TRUE, VoidOp, I32Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(e.epilog_label(), CodeGenerator::T_NEAR);
  }
};
struct RETURN_TRUE_I64
    : Sequence<RETURN_TRUE_I64, I<OPCODE_RETURN_TRUE, VoidOp, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(e.epilog_label(), CodeGenerator::T_NEAR);
  }
};
struct RETURN_TRUE_F32
    : Sequence<RETURN_TRUE_F32, I<OPCODE_RETURN_TRUE, VoidOp, F32Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    e.jnz(e.epilog_label(), CodeGenerator::T_NEAR);
  }
};
struct RETURN_TRUE_F64
    : Sequence<RETURN_TRUE_F64, I<OPCODE_RETURN_TRUE, VoidOp, F64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    e.jnz(e.epilog_label(), CodeGenerator::T_NEAR);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_RETURN_TRUE, RETURN_TRUE_I8, RETURN_TRUE_I16,
                     RETURN_TRUE_I32, RETURN_TRUE_I64, RETURN_TRUE_F32,
                     RETURN_TRUE_F64);

// ============================================================================
// OPCODE_SET_RETURN_ADDRESS
// ============================================================================
struct SET_RETURN_ADDRESS
    : Sequence<SET_RETURN_ADDRESS,
               I<OPCODE_SET_RETURN_ADDRESS, VoidOp, I64Op>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.SetReturnAddress(i.src1.constant());
  }
};
EMITTER_OPCODE_TABLE(OPCODE_SET_RETURN_ADDRESS, SET_RETURN_ADDRESS);

// ============================================================================
// OPCODE_BRANCH
// ============================================================================
struct BRANCH : Sequence<BRANCH, I<OPCODE_BRANCH, VoidOp, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.jmp(i.src1.value->name, e.T_NEAR);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_BRANCH, BRANCH);

// ============================================================================
// OPCODE_BRANCH_TRUE
// ============================================================================
struct BRANCH_TRUE_I8
    : Sequence<BRANCH_TRUE_I8, I<OPCODE_BRANCH_TRUE, VoidOp, I8Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_TRUE_I16
    : Sequence<BRANCH_TRUE_I16, I<OPCODE_BRANCH_TRUE, VoidOp, I16Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_TRUE_I32
    : Sequence<BRANCH_TRUE_I32, I<OPCODE_BRANCH_TRUE, VoidOp, I32Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_TRUE_I64
    : Sequence<BRANCH_TRUE_I64, I<OPCODE_BRANCH_TRUE, VoidOp, I64Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jnz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_TRUE_F32
    : Sequence<BRANCH_TRUE_F32, I<OPCODE_BRANCH_TRUE, VoidOp, F32Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    e.jnz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_TRUE_F64
    : Sequence<BRANCH_TRUE_F64, I<OPCODE_BRANCH_TRUE, VoidOp, F64Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    e.jnz(i.src2.value->name, e.T_NEAR);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_BRANCH_TRUE, BRANCH_TRUE_I8, BRANCH_TRUE_I16,
                     BRANCH_TRUE_I32, BRANCH_TRUE_I64, BRANCH_TRUE_F32,
                     BRANCH_TRUE_F64);

// ============================================================================
// OPCODE_BRANCH_FALSE
// ============================================================================
struct BRANCH_FALSE_I8
    : Sequence<BRANCH_FALSE_I8, I<OPCODE_BRANCH_FALSE, VoidOp, I8Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_FALSE_I16
    : Sequence<BRANCH_FALSE_I16,
               I<OPCODE_BRANCH_FALSE, VoidOp, I16Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_FALSE_I32
    : Sequence<BRANCH_FALSE_I32,
               I<OPCODE_BRANCH_FALSE, VoidOp, I32Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_FALSE_I64
    : Sequence<BRANCH_FALSE_I64,
               I<OPCODE_BRANCH_FALSE, VoidOp, I64Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.test(i.src1, i.src1);
    e.jz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_FALSE_F32
    : Sequence<BRANCH_FALSE_F32,
               I<OPCODE_BRANCH_FALSE, VoidOp, F32Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    e.jz(i.src2.value->name, e.T_NEAR);
  }
};
struct BRANCH_FALSE_F64
    : Sequence<BRANCH_FALSE_F64,
               I<OPCODE_BRANCH_FALSE, VoidOp, F64Op, LabelOp>> {
  static void Emit(X64Emitter& e, const EmitArgType& i) {
    e.vptest(i.src1, i.src1);
    e.jz(i.src2.value->name, e.T_NEAR);
  }
};
EMITTER_OPCODE_TABLE(OPCODE_BRANCH_FALSE, BRANCH_FALSE_I8, BRANCH_FALSE_I16,
                     BRANCH_FALSE_I32, BRANCH_FALSE_I64, BRANCH_FALSE_F32,
                     BRANCH_FALSE_F64);

}  // namespace x64
}  // namespace backend
}  // namespace cpu
}  // namespace xe
