#include <stdio.h>
#include "emulator.h"
#define KB 1024
#define MEMSIZE (size_t)1*KB


int main(int argc, char* argv[]) {

	FILE* inp;
	inp = fopen(argv[1], "rb");
	if (inp == NULL) {
		printf("File open failed.\n");
		return -1;
	}

	Emulator emul;
	emul.init(MEMSIZE, inp);


	fclose(inp);


	emul.inst.rip += 0x3c;
	while (emul.inst.rip < MEMSIZE) {
		uint8_t opcode = emul.nextInst();
		printf("x%02X\n", opcode);
		emul.inst.ex_op(opcode);
		emul.printRegs();
		if (emul.inst.rip == 0x00) break;
	}
	

	emul.mem.freeMem();




}
