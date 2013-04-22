#ifndef CPU_H_
#define CPU_H_

#include <string>
#include <utility>
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
	enum WriteType{
		REGISTER_WRITE,
		MEMORY_WRITE
	};
private:
	const bool usesBypassing;
	std::pair<int,WriteType> dirtyRegs[4];
	std::map<std::string,int> idStr;
	Stage* pipeline[5];
	IF* iF;
	IDRF* idrf;
	EX* ex;
	DEM* dem;
	WB* wb;
	Memory *instMem, *dataMem;

	bool pcIsDirty;
	bool pcIsDirtyNext;

public:
	Register* reg;
	explicit CPU(Register* reg, Memory* instMem, Memory* dataMem, bool usesBypassing);
	~CPU();
	void exec();
	bool isRegDirty(int num);
	bool isPcDirty();
	u32 getReg(int num);
	u32 getPc();
	void setRegDirty(int num, WriteType writeType);
	void setPcDirty();
	std::string getPipelineState(int num);

	friend class IF;
	friend class IDRF;
	friend class EX;
	friend class DEM;
	friend class WB;
};


#endif /* CPU_H_ */
