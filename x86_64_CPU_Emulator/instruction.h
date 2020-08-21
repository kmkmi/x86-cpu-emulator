#pragma once
#include <stdint.h>
#include "memory.h"


class Instruction {
private:


public:

	Memory mem;
	uint64_t regs[16];  // rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8~r15
	uint64_t rip;


	void init(uint64_t rip, uint64_t rsp, Memory mem);



};

