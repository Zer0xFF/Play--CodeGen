#include "Jitter_CodeGen_x86.h"

using namespace Jitter;

template <typename MDOP>
void CCodeGen_x86::Emit_Md_Avx_RegRegReg(const STATEMENT& statement)
{
	auto dst = statement.dst->GetSymbol().get();
	auto src1 = statement.src1->GetSymbol().get();
	auto src2 = statement.src2->GetSymbol().get();
	((m_assembler).*(MDOP::OpVoAvx()))(m_mdRegisters[dst->m_valueLow], m_mdRegisters[src1->m_valueLow], 
		CX86Assembler::MakeXmmRegisterAddress(m_mdRegisters[src2->m_valueLow]));
}

void CCodeGen_x86::Emit_Md_Avx_Mov_RegVar(const STATEMENT& statement)
{
	auto dst = statement.dst->GetSymbol().get();
	auto src1 = statement.src1->GetSymbol().get();

	m_assembler.VmovapsVo(m_mdRegisters[dst->m_valueLow], MakeVariable128SymbolAddress(src1));
}

void CCodeGen_x86::Emit_Md_Avx_Mov_MemReg(const STATEMENT& statement)
{
	auto dst = statement.dst->GetSymbol().get();
	auto src1 = statement.src1->GetSymbol().get();

	m_assembler.VmovapsVo(MakeMemory128SymbolAddress(dst), m_mdRegisters[src1->m_valueLow]);
 }

#define MD_AVX_CONST_MATCHERS_3OPS(MDOP_CST, MDOP) \
	{ MDOP_CST, MATCH_REGISTER128, MATCH_REGISTER128, MATCH_REGISTER128, &CCodeGen_x86::Emit_Md_Avx_RegRegReg<MDOP> },
//	{ MDOP_CST, MATCH_REGISTER128, MATCH_MEMORY128,   MATCH_REGISTER128, &CCodeGen_x86::Emit_Md_RegMemReg<MDOP> }, \
//	{ MDOP_CST, MATCH_REGISTER128, MATCH_VARIABLE128, MATCH_VARIABLE128, &CCodeGen_x86::Emit_Md_RegVarVar<MDOP> }, \
//	{ MDOP_CST, MATCH_MEMORY128,   MATCH_VARIABLE128, MATCH_VARIABLE128, &CCodeGen_x86::Emit_Md_MemVarVar<MDOP> },

CCodeGen_x86::CONSTMATCHER CCodeGen_x86::g_mdAvxConstMatchers[] = 
{
	MD_AVX_CONST_MATCHERS_3OPS(OP_MD_ADD_W, MDOP_ADDW)

	{ OP_MOV, MATCH_REGISTER128, MATCH_VARIABLE128, MATCH_NIL, &CCodeGen_x86::Emit_Md_Avx_Mov_RegVar, },
	{ OP_MOV, MATCH_MEMORY128,   MATCH_REGISTER128, MATCH_NIL, &CCodeGen_x86::Emit_Md_Avx_Mov_MemReg, },

	{ OP_MOV, MATCH_NIL,         MATCH_NIL,         MATCH_NIL, nullptr },
};