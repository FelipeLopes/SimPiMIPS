#ifndef PRESENTER_H_
#define PRESENTER_H_

#include "main_window.h"
#include "cpu.h"

class MainWindow;

class Presenter{
public:
	Presenter(MainWindow* mainWindow, CPU* cpu);
	void populateWindow();
	void advanceCPU();
	void getInputFile();
private:
	MainWindow* mainWindow;
	CPU* cpu;
};


#endif /* PRESENTER_H_ */
