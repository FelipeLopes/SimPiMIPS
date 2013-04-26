#include <cstdio>
#include <cassert>
#include "cpu.h"

CPU::CPU(Register* reg, Memory* instMem, Memory* dataMem, bool usesBypassing, int progSize):
		reg(reg),instMem(instMem),dataMem(dataMem),
		usesBypassing(usesBypassing),progSize(progSize){
	currentClock = 0;
	numInstructions = 0;
	executionFinished = false;

	pipeline[0] = iF = new IF(this);
	pipeline[1] = idrf = new IDRF(this);
	pipeline[2] = ex = new EX(this);
	pipeline[3] = dem = new DEM(this);
	pipeline[4] = wb = new WB(this);

	for (int i=0; i<4; i++) dirtyRegs[i].first=-2;
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

void CPU::setPcDirty(){
	dirtyRegs[0] = std::make_pair(-1,REGISTER_WRITE);
}

bool CPU::isPcDirty(){
	if (!usesBypassing){
		for (int i=1; i<4; i++) if (dirtyRegs[i].first==-1) return true;
		return false;
	}
	if (dirtyRegs[1].first==-1) return true;
	return false;
}

u32 CPU::getPc(){
	if (!usesBypassing) return reg->readPC();
	if (usesBypassing && dirtyRegs[1].first==-1) return ex->result;
	if (usesBypassing && dirtyRegs[2].first==-1) return dem->result;
	return reg->readPC();
}

void CPU::exec(){
	if (executionFinished) return;
	currentClock++;
	dirtyRegs[3]=dirtyRegs[2];
	dirtyRegs[2]=dirtyRegs[1];
	dirtyRegs[1]=dirtyRegs[0];
	dirtyRegs[0].first=-2;
	for (int i=4; i>=0; i--) pipeline[i]->exec();
	int state = getPipelineState(4);
	if (state==Stage::STATE_OUT_OF_PROGRAM){
		--currentClock;
		executionFinished=true;
	}
	else if (state!=Stage::STATE_STALL) numInstructions++;
}

int CPU::getPipelineState(int num){
	return pipeline[num]->getState();
}

int CPU::getCurrentClock(){
	return currentClock;
}

int CPU::getNumInstructions(){
	return numInstructions;
}

void CPU::writeMemory(u32 addr, u32 val){
	if (accessQueue.size()==4) accessQueue.pop_back();
	accessQueue.push_front(std::make_pair(std::make_pair(std::make_pair(addr,'W'),val),currentClock));
	dataMem->write(addr,val);
}

u32 CPU::readMemory(u32 addr){
	u32 val = dataMem->read(addr);
	if (accessQueue.size()==4) accessQueue.pop_back();
	accessQueue.push_front(std::make_pair(std::make_pair(std::make_pair(addr,'R'),val),currentClock));
	return val;
}

int CPU::getAccessClock(int pos){
	return accessQueue[pos].second;
}

u32 CPU::getAccessAddress(int pos){
	return accessQueue[pos].first.first.first;
}

std::string CPU::getAccessResult(int pos){
	std::string s = "";
	s += accessQueue[pos].first.first.second;
	s += " ";
	char str[40];
	sprintf(str,"%d",accessQueue[pos].first.second);
	s += str;
	return s;
}

int CPU::getAccessQueueSize(){
	return accessQueue.size();
}

bool CPU::isExecutionFinished(){
	return executionFinished;
}
