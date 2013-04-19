#include <cstdio>
#include "cpu.h"

CPU::CPU(Register* reg, Memory* instMem, Memory* dataMem){
	this->reg = reg;
	this->instMem = instMem;
	this->dataMem = dataMem;

	idStr.clear();
	idStr["add"] = 0;
	idStr["addi"] = 1;
	idStr["beq"] = 2;
	idStr["ble"] = 3;
	idStr["bne"] = 4;
	idStr["jmp"] = 5;
	idStr["lw"] = 6;
	idStr["mul"] = 7;
	idStr["nop"] = 8;
	idStr["sub"] = 9;
	idStr["sw"] = 10;

	pipeline[0] = iF = new IF(this);
	pipeline[1] = idrf = new IDRF(this);
	pipeline[2] = ex = new EX(this);
	pipeline[3] = dem = new DEM(this);
	pipeline[4] = wb = new WB(this);

	for (int i=0; i<32; i++) regIsDirty[i]=regIsDirtyNext[i]=false;
	pcIsDirty=pcIsDirtyNext=false;
}

CPU::~CPU(){
	for (int i=0; i<5; i++) delete pipeline[i];
}

void CPU::exec(){
	std::string s[5];
	for (int i=0; i<32; i++) regIsDirtyNext[i]=regIsDirty[i];
	pcIsDirtyNext=pcIsDirty;
	for (int i=4; i>=0; i--){
		pipeline[i]->exec();
		s[i]=pipeline[i]->getState();
	}
	for (int i=0; i<32; i++) regIsDirty[i]=regIsDirtyNext[i];
	pcIsDirty = pcIsDirtyNext;
}
