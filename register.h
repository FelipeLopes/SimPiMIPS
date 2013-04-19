#ifndef REGISTER_H_
#define REGISTER_H_

#include <cstring>
#include "typedefs.h"

class Register{
private:
	u32 reg[31];
	u32 pc;
public:
	Register();
	u32 read(int number);
	void write(int number, int val);
	u32 readPC();
	void writePC(int val);
};


#endif /* REGISTER_H_ */
