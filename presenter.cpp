#include "main_window.h"
#include "cpu.h"
#include "presenter.h"

Presenter::Presenter(MainWindow* mainWindow, CPU* cpu):
		mainWindow(mainWindow),cpu(cpu){
	mainWindow->pipelineDisplay->SetCellValue(0,0,_("IF"));
	mainWindow->pipelineDisplay->SetCellValue(0,1,_("IDRF"));
	mainWindow->pipelineDisplay->SetCellValue(0,2,_("EX"));
	mainWindow->pipelineDisplay->SetCellValue(0,3,_("DEM"));
	mainWindow->pipelineDisplay->SetCellValue(0,4,_("WB"));
}

void Presenter::populateWindow(){
	for (int i=0; i<5; i++){
		wxString str(cpu->getPipelineState(i).c_str(),wxConvUTF8);
		mainWindow->pipelineDisplay->SetCellValue(1,i,str);
	}
}

void Presenter::advanceCPU(){
	cpu->exec();
	populateWindow();
}
