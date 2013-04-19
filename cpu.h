#ifndef CPU_H_
#define CPU_H_

#include <string>
#include <map>
#include "typedefs.h"
#include "stage.h"

class Stage;
class IF;
class IDRF;
class EX;
class DEM;
class WB;

class CPU{
private:
	std::map<std::string,int> idStr;
	IF* iF;
	IDRF* idrf;
	EX* ex;
	DEM* dem;
	WB* wb;
	Stage* pipeline[5];
	Memory *instMem, *dataMem;
	Register* reg;

	bool regIsDirty[32];
	bool regIsDirtyNext[32];
	bool pcIsDirty;
	bool pcIsDirtyNext;

public:
	explicit CPU(Register* reg, Memory* instMem, Memory* dataMem);
	~CPU();
	void exec();
	friend class IF;
	friend class IDRF;
	friend class EX;
	friend class DEM;
	friend class WB;
};


#endif /* CPU_H_ */
