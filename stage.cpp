#include <cstdio>
#include <cassert>
#include <string>
#include <sstream>
#include "stage.h"
#include "cpu.h"

Stage::Stage(CPU* cpu):cpu(cpu){
	isBlocked = false;
	executed = false;
	isIdle = true;
}

Stage::~Stage(){

}

int Stage::getState(){
	if (!executed) return Stage::STATE_STALL;
	else if (instPos<0 || instPos>=4*cpu->progSize) return Stage::STATE_OUT_OF_PROGRAM;
	return instPos;
}

IF::IF(CPU* cpu):Stage(cpu){
	isIdle = false;
}

void IF::exec() {
	executed=false;
	if (cpu->idrf.isBlocked || cpu->isPcDirty()){
		isBlocked=true;
		return;
	}
	instPos = cpu->getPc();
	currentInst = cpu->instMem->read(cpu->getPc());
	cpu->reg->writePC(cpu->getPc() + 4);
	isBlocked=false;
	cpu->idrf.isIdle=false;
	executed=true;
}

IDRF::IDRF(CPU* cpu):Stage(cpu){

}

void IDRF::exec() {
	executed=false;
	if(isIdle) return;
	if (cpu->isPcDirty()) isIdle=true;
	if (cpu->ex.isBlocked)	isBlocked=true;
	if (cpu->isPcDirty() || cpu->ex.isBlocked) return;

	u32 instr = cpu->iF.currentInst;
	op = instr >> 26;
	switch (op) {
	case 0b000000:
		format = 'R';
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		rd = (instr >> 11) & 0x1F;
		funct = instr & 0x3F;
		switch (funct) {
		case 0b000000:
			command = CPU::INST_NOP;
			break;
		case 0b011000:
			command = CPU::INST_MUL;
			break;
		case 0b100000:
			command = CPU::INST_ADD;
			break;
		case 0b100010:
			command = CPU::INST_SUB;
			break;
		default:
			assert(0);
			break;
		}
		break;
	case 0b000010:
		format = 'J';
		command = CPU::INST_JMP;
		targetAddr = instr & ((1 << 26) - 1);
		break;
	case 0b000100:
		format = 'I';
		command = CPU::INST_BNE;
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b000101:
		format = 'I';
		command = CPU::INST_BEQ;
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b000111:
		format = 'I';
		command = CPU::INST_BLE;
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b001000:
		format = 'I';
		command = CPU::INST_ADDI;
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b100011:
		format = 'I';
		command = CPU::INST_LW;
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b101011:
		format = 'I';
		command = CPU::INST_SW;
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	}

	bool readSuccessful;
	switch(command){
	case CPU::INST_ADD:
	case CPU::INST_MUL:
	case CPU::INST_SUB:
	case CPU::INST_SW:
		readSuccessful = !cpu->isRegDirty(rs) && !cpu->isRegDirty(rt);
		break;
	case CPU::INST_ADDI:
	case CPU::INST_LW:
		readSuccessful = !cpu->isRegDirty(rs);
		break;
	case CPU::INST_BEQ:
	case CPU::INST_BLE:
	case CPU::INST_BNE:
		readSuccessful = !cpu->isRegDirty(rs) && !cpu->isRegDirty(rt) && !cpu->isPcDirty();
		break;
	case CPU::INST_JMP:
	case CPU::INST_NOP:
		readSuccessful = true;
		break;
	default:
		assert(0);
		break;
	}
	if (!readSuccessful){
		isBlocked = true;
		return;
	}
	switch(command){
	case CPU::INST_ADD:
	case CPU::INST_MUL:
	case CPU::INST_SUB:
		cpu->setRegDirty(rd,CPU::REGISTER_WRITE);
		break;
	case CPU::INST_ADDI:
		cpu->setRegDirty(rt,CPU::REGISTER_WRITE);
		break;
	case CPU::INST_LW:
		cpu->setRegDirty(rt,CPU::MEMORY_WRITE);
		break;
	case CPU::INST_BEQ:
	case CPU::INST_BLE:
	case CPU::INST_BNE:
	case CPU::INST_JMP:
		cpu->setPcDirty();
		break;
	case CPU::INST_SW:
	case CPU::INST_NOP:
		break;
	default:
		assert(0);
		break;
	}

	switch (format) {
	case 'R':
	case 'I':
		valRs = cpu->getReg(rs);
		valRt = cpu->getReg(rt);
		valPC = cpu->getPc();
		break;
	case 'J':
		break;
	default:
		assert(0);
		break;
	}

	isBlocked = false;
	isIdle=true;
	cpu->ex.isIdle=false;

	instPos = cpu->iF.instPos;
	executed=true;
}

EX::EX(CPU* cpu):Stage(cpu){
	ticksToFinish=0;
}

void EX::exec() {
	executed=false;
	IDRF& idrf = cpu->idrf;
	command = idrf.command;
	rd = idrf.rd;
	rt = idrf.rt;
	valRt = idrf.valRt;
	bool cond;

	if(isIdle) return;
	if (ticksToFinish==0 &&	command==CPU::INST_MUL) ticksToFinish = 2;
	else if (ticksToFinish==0) ticksToFinish = 1;
	if(ticksToFinish>0) --ticksToFinish;
	if (cpu->dem.isBlocked || ticksToFinish>0){
		isBlocked=true;
		return;
	}
	switch (command) {
	case CPU::INST_ADD: //add
		result = idrf.valRs + idrf.valRt;
		break;
	case CPU::INST_ADDI: //addi
		result = idrf.valRs + idrf.immed;
		break;
	case CPU::INST_BEQ: //beq
		cond = (static_cast<i32>(idrf.valRs) ==
				static_cast<i32>(idrf.valRt));
		if (cond) result = idrf.valPC + idrf.immed;
		else result = idrf.valPC;
		break;
	case CPU::INST_BLE: //ble
		cond = (static_cast<i32>(idrf.valRs) <=
				static_cast<i32>(idrf.valRt));
		if (cond) result = idrf.immed;
		else result = idrf.valPC;
		break;
	case CPU::INST_BNE: //bne
		cond = (static_cast<i32>(idrf.valRs) !=
				static_cast<i32>(idrf.valRt));
		if (cond) result = idrf.valPC + idrf.immed;
		else result = idrf.valPC;
		break;
	case CPU::INST_JMP: //jmp
		result = idrf.targetAddr;
		break;
	case CPU::INST_LW: //lw
		result = idrf.valRs + idrf.immed;
		break;
	case CPU::INST_MUL: //mul
		result = idrf.valRs * idrf.valRt;
		break;
	case CPU::INST_NOP: //nop
		//Do nothing
		break;
	case CPU::INST_SUB: //sub
		result = idrf.valRs - idrf.valRt;
		break;
	case CPU::INST_SW: //sw
		result = idrf.valRs + idrf.immed;
		break;
	}

	isBlocked=false;
	isIdle=true;
	cpu->dem.isIdle=false;
	instPos = cpu->idrf.instPos;
	executed=true;
}

DEM::DEM(CPU* cpu):Stage(cpu){

}

void DEM::exec() {
	executed=false;
	EX& ex = cpu->ex;
	rd = ex.rd;
	rt = ex.rt;
	result = ex.result;
	command = ex.command;

	if (isIdle) return;
	if (cpu->wb.isBlocked){
		isBlocked=true;
		return;
	}
	if (command == CPU::INST_LW){
		result = cpu->readMemory(ex.result);
	}
	if (command == CPU::INST_SW) {
		cpu->writeMemory(ex.result, ex.valRt);
	}
	isBlocked = false;
	isIdle=true;
	cpu->wb.isIdle=false;

	instPos = cpu->ex.instPos;
	executed=true;
}

WB::WB(CPU* cpu):Stage(cpu){

}

void WB::exec() {
	executed=false;
	Register* reg = cpu->reg;
	DEM& dem = cpu->dem;

	if (isIdle) return;
	switch (dem.command) {
	case CPU::INST_ADD:
	case CPU::INST_MUL:
	case CPU::INST_SUB:
		reg->write(dem.rd,dem.result);
		break;
	case CPU::INST_ADDI:
	case CPU::INST_LW:
		reg->write(dem.rt,dem.result);
		break;
	case CPU::INST_BEQ:
	case CPU::INST_BLE:
	case CPU::INST_BNE:
	case CPU::INST_JMP:
		if (!cpu->usesBypassing) reg->writePC(dem.result);
		break;
	case CPU::INST_NOP:
	case CPU::INST_SW:
		//Do nothing
		break;
	default:
		assert(0);
		break;
	}
	isIdle = true;
	instPos = cpu->dem.instPos;
	executed=true;
}
