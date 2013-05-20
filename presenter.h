#ifndef PRESENTER_H_
#define PRESENTER_H_

#include <vector>
#include <string>
#include "typedefs.h"
#include "main_window.h"
#include "cpu.h"

class Presenter{
public:
	Presenter();
	~Presenter();
	void advanceCPU(int n);
	wxString getFilePath();
	void initializeCPU();
	void dumpOutput();
	MainWindow* getMainWindow();
private:
    void cleanUp();
	std::vector<std::string> instDesc;
	MainWindow* mainWindow;
	CPU* cpu;
	Memory *instMem,*dataMem;
	Register* reg;
};


#endif /* PRESENTER_H_ */
