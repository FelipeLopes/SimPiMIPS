#ifndef STAGE_H_
#define STAGE_H_

#include "../util/typedefs.h"
#include "memory.h"
#include "register.h"

class CPU;

class Stage{
public:
	enum {
		STATE_OUT_OF_PROGRAM = -2,
		STATE_STALL = -1
	};
	explicit Stage(CPU* cpu);
	int getState();
	virtual ~Stage();
	virtual void exec() = 0;
	u32 instPos;
	bool isBlocked,isIdle;
	bool executed;
protected:
	CPU* cpu;
};

class IF: public Stage{
public:
	explicit IF(CPU* cpu);
	void exec();
	u32 currentInst;
};

class IDRF: public Stage{
public:
	explicit IDRF(CPU* cpu);
	void exec();
	int command;
	char format;
	int op,rs,rt,rd,funct,targetAddr;
	i16 immed;
	u32 valRs,valRt,valPC;
};

class EX: public Stage{
public:
	explicit EX(CPU* cpu);
	void exec();
	u32 result;
	int command;
	int rd,rt;
	u32 valRt;
private:
	int ticksToFinish;
};

class DEM: public Stage{
public:
	explicit DEM(CPU* cpu);
	void exec();
	int rd,rt;
	u32 result;
	int command;
};

class WB: public Stage{
public:
	explicit WB(CPU* cpu);
	void exec();
};

#endif /* STAGE_H_ */
