#ifndef PRESENTER_H_
#define PRESENTER_H_

#include <vector>
#include <string>
#include "typedefs.h"
#include "main_window.h"
#include "cpu.h"

class MainWindow;

class Presenter{
public:
	Presenter();
	void advanceCPU(int n);
	wxString getFilePath();
	void initializeCPU();
	void dumpOutput();
	MainWindow* getMainWindow();
private:
	std::vector<std::string> instDesc;
	Memory *instMem,*dataMem;
	Register *reg;
	MainWindow* mainWindow;
	CPU* cpu;
};


#endif /* PRESENTER_H_ */
