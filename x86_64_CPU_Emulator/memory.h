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


};
#endif