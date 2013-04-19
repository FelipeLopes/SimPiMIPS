#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstring>
#include "typedefs.h"

class Memory{
private:
	u32 mem[0x8000];
public:
	Memory();
	u32 read(int address);
	void write(int address, int val);
};


#endif /* MEMORY_H_ */
