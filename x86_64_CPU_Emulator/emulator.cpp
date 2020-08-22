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

void Emulator::printRegs() {
	printf("rax = 0x%016x (%d)\n", inst.regs[0], inst.regs[0]);
	printf("rcx = 0x%016x (%d)\n", inst.regs[1], inst.regs[1]);
	printf("rdx = 0x%016x (%d)\n", inst.regs[2], inst.regs[2]);
	printf("rbx = 0x%016x (%d)\n", inst.regs[3], inst.regs[3]);
	printf("rsp = 0x%016x (%d)\n", inst.regs[4], inst.regs[4]);
	printf("rbp = 0x%016x (%d)\n", inst.regs[5], inst.regs[5]);
	printf("rsi = 0x%016x (%d)\n", inst.regs[6], inst.regs[6]);
	printf("rdi = 0x%016x (%d)\n", inst.regs[7], inst.regs[7]);
	for (int i = 8; i < 16; i++) {
		printf("r%2d = 0x%016x (%d)\n", i, inst.regs[i], inst.regs[i]);
	}
	printf("rip = 0x%016x (%d)\n", inst.rip, inst.rip);
}

