#include <cstdio>
#include <cassert>
#include "cpu.h"

CPU::CPU(Register* reg, Memory* instMem, Memory* dataMem, bool usesBypassing):
		usesBypassing(usesBypassing){
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

	for (int i=0; i<4; i++) dirtyRegs[i].first=-1;
	pcIsDirty=pcIsDirtyNext=false;
}

CPU::~CPU(){
	for (int i=0; i<5; i++) delete pipeline[i];
}

void CPU::setRegDirty(int num, WriteType writeType){
	dirtyRegs[0]=std::make_pair(num,writeType);
}

bool CPU::isRegDirty(int num){
	if (!usesBypassing){
		for (int i=1; i<4; i++) if (dirtyRegs[i].first==num) return true;
		return false;
	}
	return dirtyRegs[1].first==num && dirtyRegs[1].second==MEMORY_WRITE;
}

u32 CPU::getReg(int num){
	if (!usesBypassing) return reg->read(num);
	if (dirtyRegs[1].first==num) return ex->result;
	if (dirtyRegs[2].first==num) return dem->result;
	return reg->read(num);
}

void CPU::exec(){
	pcIsDirtyNext=pcIsDirty;
	dirtyRegs[3]=dirtyRegs[2];
	dirtyRegs[2]=dirtyRegs[1];
	dirtyRegs[1]=dirtyRegs[0];
	dirtyRegs[0].first=-1;
	for (int i=4; i>=0; i--) pipeline[i]->exec();
	pcIsDirty = pcIsDirtyNext;
}

std::string CPU::getPipelineState(int num){
	return pipeline[num]->getState();
}
