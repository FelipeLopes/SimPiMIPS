#include <cassert>
#include <string>
#include <sstream>
#include "stage.h"

Stage::Stage(CPU* cpu){
	isBlocked = false;
	executed = false;
	isIdle = true;
	this->cpu = cpu;
}

Stage::~Stage(){

}

std::string Stage::getState(){
	if (!executed) return "Bubble";
	else return stateString;
}

IF::IF(CPU* cpu):Stage(cpu){
	isIdle = false;
}

void IF::exec() {
	executed=false;
	if (cpu->idrf->isBlocked || cpu->pcIsDirty){
		this->isBlocked=true;
		return;
	}

	currentInst = cpu->instMem->read(cpu->reg->readPC());
	cpu->reg->writePC(cpu->reg->readPC() + 4);

	this->isBlocked = false;
	cpu->idrf->isIdle=false;

	stateString = "I";
	std::stringstream ss;
	ss<<(cpu->reg->readPC()/4-1);
	stateString += ss.str();
	executed=true;
}

IDRF::IDRF(CPU* cpu):Stage(cpu){

}

void IDRF::exec() {
	executed=false;
	if(this->isIdle) return;

	if (cpu->pcIsDirty)	this->isIdle=true;
	if (cpu->ex->isBlocked)	this->isBlocked=true;

	if (cpu->pcIsDirty || cpu->ex->isBlocked) return;

	u32 instr = cpu->iF->currentInst;
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
			command = "nop";
			break;
		case 0b011000:
			command = "mul";
			break;
		case 0b100000:
			command = "add";
			break;
		case 0b100010:
			command = "sub";
			break;
		default:
			assert(0);
			break;
		}
		break;
	case 0b000010:
		format = 'J';
		command = "jmp";
		targetAddr = instr & ((1 << 26) - 1);
		break;
	case 0b000100:
		format = 'I';
		command = "bne";
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b000101:
		format = 'I';
		command = "beq";
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b000111:
		format = 'I';
		command = "ble";
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b001000:
		format = 'I';
		command = "addi";
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b100011:
		format = 'I';
		command = "lw";
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	case 0b101011:
		format = 'I';
		command = "sw";
		rs = (instr >> 21) & 0x1F;
		rt = (instr >> 16) & 0x1F;
		immed = instr & ((1 << 16) - 1);
		break;
	}

	bool readSuccessful;

	switch(cpu->idStr[command]){
	case 0: //add
	case 7: //mul
	case 9: //sub
	case 10://sw
		readSuccessful = !cpu->regIsDirty[rs] && !cpu->regIsDirty[rt];
		break;
	case 1: //addi
	case 6: //lw
		readSuccessful = !cpu->regIsDirty[rs];
		break;
	case 2: //beq
	case 3: //ble
	case 4: //bne
		readSuccessful = !cpu->regIsDirty[rs] && !cpu->regIsDirty[rt] && !cpu->pcIsDirty;
		break;
	default: //jmp, nop
		readSuccessful = true;
		break;
	}

	if (!readSuccessful){
		this->isBlocked = true;
		return;
	}

	switch(cpu->idStr[command]){
	case 0: //add
	case 7: //mul
	case 9: //sub
		cpu->regIsDirtyNext[rd] = true;
		break;
	case 1: //addi
	case 6: //lw
		cpu->regIsDirtyNext[rt] = true;
		break;
	case 2: //beq
	case 3: //ble
	case 4: //bne
	case 5: //jmp
		cpu->pcIsDirtyNext = true;
		break;
	default: //sw, nop
		break;
	}

	switch (format) {
	case 'R':
	case 'I':
		valRs = cpu->reg->read(rs);
		valRt = cpu->reg->read(rt);
		valPC = cpu->reg->readPC();
		break;
	case 'J':
		break;
	default:
		assert(0);
		break;
	}

	this->isBlocked = false;
	this->isIdle=true;
	cpu->ex->isIdle=false;

	this->stateString = cpu->iF->stateString;
	executed=true;
}

EX::EX(CPU* cpu):Stage(cpu){

}

void EX::exec() {
	executed=false;
	IDRF* idrf = cpu->idrf;
	command = idrf->command;
	rd = idrf->rd;
	rt = idrf->rt;
	valRt = idrf->valRt;
	valPC = idrf->valPC;

	if(this->isIdle) return;

	assert(cpu->idStr.find(cpu->idrf->command)!=cpu->idStr.end());

	if (ticksToFinish==0 &&
			idrf->command.compare("mul")==0) ticksToFinish = 2;
	else if (ticksToFinish==0) ticksToFinish = 1;

	if(ticksToFinish>0) --ticksToFinish;

	if (cpu->dem->isBlocked || ticksToFinish>0){
		this->isBlocked=true;
		return;
	}

	switch (cpu->idStr[idrf->command]) {
	case 0: //add
		result = idrf->valRs + idrf->valRt;
		break;
	case 1: //addi
		result = idrf->valRs + idrf->immed;
		break;
	case 2: //beq
		cond = (static_cast<i32>(idrf->valRs) ==
				static_cast<i32>(idrf->valRt));
		result = idrf->valPC + idrf->immed;
		break;
	case 3: //ble
		cond = (static_cast<i32>(idrf->valRs) <=
				static_cast<i32>(idrf->valRt));
		result = idrf->immed;
		break;
	case 4: //bne
		cond = (static_cast<i32>(idrf->valRs) !=
				static_cast<i32>(idrf->valRt));
		result = idrf->valPC + idrf->immed;
		break;
	case 5: //jmp
		result = idrf->targetAddr;
		break;
	case 6: //lw
		result = idrf->valRs + idrf->immed;
		break;
	case 7: //mul
		result = idrf->valRs * idrf->valRt;
		break;
	case 8: //nop
		//Do nothing
		break;
	case 9: //sub
		result = idrf->valRs - idrf->valRt;
		break;
	case 10: //sw
		result = idrf->valRs + idrf->immed;
		break;
	}

	this->isBlocked = false;
	this->isIdle=true;
	cpu->dem->isIdle=false;

	this->stateString = cpu->idrf->stateString;
	executed=true;
}

DEM::DEM(CPU* cpu):Stage(cpu){

}

void DEM::exec() {
	executed=false;
	EX* ex = cpu->ex;
	Memory* mem = cpu->dataMem;
	rd = ex->rd;
	rt = ex->rt;
	result = ex->result;
	cond = ex->cond;
	command = ex->command;
	valPC = ex->valPC;

	if (this->isIdle) return;

	if (cpu->wb->isBlocked){
		this->isBlocked=true;
		return;
	}

	if (command.compare("lw") == 0) {
		result = mem->read(ex->result);
	}

	if (command.compare("sw") == 0) {
		mem->write(ex->result, ex->valRt);
	}

	this->isBlocked = false;
	this->isIdle=true;
	cpu->wb->isIdle=false;

	this->stateString = cpu->ex->stateString;
	executed=true;
}

WB::WB(CPU* cpu):Stage(cpu){

}

void WB::exec() {
	executed=false;
	Register* reg = cpu->reg;
	DEM* dem = cpu->dem;

	if(this->isIdle) return;

	assert(cpu->idStr.find(cpu->dem->command)!=cpu->idStr.end());
	switch (cpu->idStr[dem->command]) {
	case 0: //add
	case 7: //mul
	case 9: //sub
		reg->write(dem->rd,dem->result);
		cpu->regIsDirtyNext[dem->rd] = false;
		break;
	case 1: //addi
	case 6: //lw
		reg->write(dem->rt,dem->result);
		cpu->regIsDirtyNext[dem->rt] = false;
		break;
	case 2: //beq
	case 3: //ble
	case 4: //bne
		if (dem->cond) reg->writePC(dem->result);
		else reg->writePC(dem->valPC);
		cpu->pcIsDirtyNext = false;
		break;
	case 5: //jmp
		reg->writePC(dem->result);
		cpu->pcIsDirtyNext = false;
		break;
	case 8: //nop
	case 10://sw
		//Do nothing
		break;
	}

	this->isIdle = true;

	this->stateString = cpu->dem->stateString;
	executed=true;
}