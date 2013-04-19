#include "register.h"

Register::Register(){
	memset(reg,0,sizeof(reg));
	pc=0;
}

u32 Register::read(int num){
	if (num==0) return 0;
	return reg[num-1];
}

void Register::write(int num, int val){
	if (num>0) reg[num-1]=val;
}

u32 Register::readPC(){
	return pc;
}

void Register::writePC(int val){
	pc = val;
}
