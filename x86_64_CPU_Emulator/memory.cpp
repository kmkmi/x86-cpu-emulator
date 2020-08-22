#include "memory.h"
#include <stdlib.h>
#include <stdint.h>

void Memory::init(size_t memsize) {
	this->memsize = memsize;
	this->memld = (uint8_t*)malloc(memsize);
}

void Memory::loadBinToMem(FILE* inp) {
	size_t binsize = 0;
	fseek(inp, 0L, SEEK_END); 
	binsize = ftell(inp); 
	fseek(inp, 0L, SEEK_SET); 
	fread(this->memld, binsize , 1, inp);
}

uint8_t Memory::read1byte(uint64_t offset) {
	return (uint8_t)this->memld[offset];
}


int8_t Memory::readChar(uint64_t offset) {
	return (int8_t)this->memld[offset];
}


uint32_t Memory::read4bytes(uint64_t offset) {

	return (uint32_t)this->memld[offset];
}

void Memory::write8bytes(uint64_t offset, uint64_t buf) {
	this->memld[offset] = buf;
}

void Memory::freeMem() {
	free(this->memld);
}
