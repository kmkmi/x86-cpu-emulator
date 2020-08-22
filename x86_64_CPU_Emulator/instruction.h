#pragma once
#include <stdint.h>
#include "memory.h"

#define CF 0x0000000000000001
#define ZF 0x0000000000000040


#define ADD 0
#define XOR 6



class Instruction {
private:


public:

	Memory mem;
	uint64_t regs[16];  // rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8~r15
	uint64_t rip;
	uint64_t eflag;
	uint8_t rex;
	uint8_t modrm;
	uint8_t mod, reg, rm;

	void ex_op(uint8_t op);

	uint8_t next_op();
	void readModrm();

	void (Instruction::* insts[256])(void);
	void init(uint64_t rip, uint64_t rsp, Memory mem);


	void chkMod(int op);
	void op_mod0to2(uint64_t addr, uint64_t dst, int op);
	void op_mod3(uint64_t* dst, uint64_t* src, int calc_type);

	void add_rm32_r32();

	void xor_rm32_r32();


	void op_ff();

	void op_83();

	void cmp_rm32_imm(int imm_flag);

	void mov_rm32_r32();


	void mov_eax_imm32();
	void mov_ecx_imm32();

	void jne_imm8();

	void jmp_imm8();


	void hlt();

	void nop();


	int getEflag(uint64_t flag);
	void setEflag(int flag, uint64_t eflag);
};

