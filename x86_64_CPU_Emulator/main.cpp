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




	while (emul.inst.rip < MEMSIZE) {
		uint8_t opcode = emul.nextInst();
		printf("0x%02X\n", opcode);
		//emul.exec(opcode);

		if (emul.inst.rip == 0x00) break;
	}
	


	fclose(inp);


}
