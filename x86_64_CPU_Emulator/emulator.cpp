#include "emulator.h";

void Emulator::init(size_t memsize, FILE *inp) {

	mem.init(memsize);
	mem.loadBinToMem(inp);

	inst.init(0, int(memsize), mem);
}

uint8_t Emulator::nextInst() {
	uint8_t opcode = mem.read1byte(inst.rip++);
	return opcode;
}