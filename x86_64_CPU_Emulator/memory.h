#pragma once
#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include <stdint.h>

class Memory {

private:
	uint8_t *memld;
	size_t memsize;


public: 
	void init(size_t memsize);
	void loadBinToMem(FILE* inp);
	uint8_t read1byte(uint64_t offset);
	uint32_t read4bytes(uint64_t offset);
	int8_t readChar(uint64_t offset);
	void write8bytes(uint64_t offset, uint64_t buf);
	void freeMem();
};
#endif