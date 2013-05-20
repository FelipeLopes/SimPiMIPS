#include "memory.h"

Memory::Memory(){
	memset(mem,0,sizeof(mem));
}

u32 Memory::read(int address){
	return mem[address/4];
}

void Memory::write(int address, int val){
	mem[address/4]=val;
}
