#ifndef Stage_H_
#define Stage_H_

#include <string>
#include "typedefs.h"
#include "memory.h"
#include "register.h"
#include "cpu.h"

class CPU;

class Stage{
protected:
	CPU* cpu;
public:
	std::string stateString;
	bool isBlocked,isIdle;
	bool executed;
	Stage(CPU* cpu);
	std::string getState();
	virtual ~Stage();
	virtual void exec() = 0;
};

class IF: public Stage{
public:
	u32 currentInst;
	void exec();

	explicit IF(CPU* cpu);
};

class IDRF: public Stage{
private:
	bool pcWasDirtyBefore;
public:
	std::string command;
	char format;
	int op,rs,rt,rd,funct,targetAddr;
	i16 immed;
	u32 valRs,valRt,valPC;
	void exec();

	explicit IDRF(CPU* cpu);
};

class EX: public Stage{
public:
	int ticksToFinish;
public:
	u32 result;
	bool cond;
	std::string command;
	int rd,rt;
	u32 valRt,valPC;
	void exec();

	explicit EX(CPU* cpu);
};

class DEM: public Stage{
public:
	int rd,rt;
	u32 result,valPC;
	bool cond;
	std::string command;
	void exec();

	explicit DEM(CPU* cpu);
};

class WB: public Stage{
public:
	void exec();
	explicit WB(CPU* cpu);
};

#endif /* Stage_H_ */
