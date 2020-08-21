#include "instruction.h"
#include <string.h>


void Instruction::init(uint64_t rip, uint64_t rsp, Memory mem) {

	memset(this->regs, 0, sizeof(this->regs));
	this->regs[4] = rsp;
	this->rip = rip;
	this->mem = mem;


}