#include <cstdio>
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
	char s[10];
	for (int i=0; i<32; i++){
		std::string str = "R";
		sprintf(s,"%d",i);
		str+=s;
		str+=": ";
		sprintf(s,"%d",cpu->reg->read(i));
		str+=s;
		wxString wxStr(str.c_str(),wxConvUTF8);
		mainWindow->registerDisplay->SetCellValue(i%8,i/8,wxStr);
	}
}

void Presenter::advanceCPU(){
	cpu->exec();
	populateWindow();
}
