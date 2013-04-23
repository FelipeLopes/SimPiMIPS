#ifndef PRESENTER_H_
#define PRESENTER_H_

#include "main_window.h"
#include "cpu.h"

class MainWindow;

class Presenter{
public:
	Presenter();
	void populateWindow();
	void advanceCPU();
	void getFilePath(wxTextCtrl* textCtrl);
	void init();
	MainWindow* getMainWindow();
private:
	Memory *instMem,*dataMem;
	Register *reg;
	MainWindow* mainWindow;
	CPU* cpu;
};


#endif /* PRESENTER_H_ */
