#pragma once
#include "instruction.h"
#include "memory.h"
#include <stdio.h>

class Emulator {


public:
	Instruction inst;
	Memory mem;


	void init(size_t memsize, FILE* inp);
	uint64_t getRIP();
	uint8_t nextInst();
};