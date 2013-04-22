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

	mainWindow->miscDisplay->SetCellValue(0,0,_("Clock:"));
	mainWindow->miscDisplay->SetCellValue(1,0,_("PC:"));
	mainWindow->miscDisplay->SetCellValue(2,0,_("No. instructions:"));
	mainWindow->miscDisplay->SetCellValue(3,0,_("Productivity:"));

	mainWindow->accessDisplay->SetCellValue(0,0,_("Clock"));
	mainWindow->accessDisplay->SetCellValue(0,1,_("Address"));
	mainWindow->accessDisplay->SetCellValue(0,2,_("Access"));
}

void Presenter::populateWindow(){
	for (int i=0; i<5; i++){
		wxString str(cpu->getPipelineState(i).c_str(),wxConvUTF8);
		mainWindow->pipelineDisplay->SetCellValue(1,i,str);
	}
	char s[40];
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
	sprintf(s,"%d",cpu->getCurrentClock());
	mainWindow->miscDisplay->SetCellValue(0,1,wxString(s,wxConvUTF8));
	sprintf(s,"%d",cpu->reg->readPC());
	mainWindow->miscDisplay->SetCellValue(1,1,wxString(s,wxConvUTF8));
	sprintf(s,"%d",cpu->getNumInstructions());
	mainWindow->miscDisplay->SetCellValue(2,1,wxString(s,wxConvUTF8));
	sprintf(s,"%.2lf%%",100.0*cpu->getNumInstructions()/cpu->getCurrentClock());
	mainWindow->miscDisplay->SetCellValue(3,1,wxString(s,wxConvUTF8));

	for (int i=0; i<cpu->getAccessQueueSize(); i++){
		sprintf(s,"%u",cpu->getAccessClock(i));
		mainWindow->accessDisplay->SetCellValue(i+1,0,wxString(s,wxConvUTF8));
		sprintf(s,"%u",cpu->getAccessAddress(i));
		mainWindow->accessDisplay->SetCellValue(i+1,1,wxString(s,wxConvUTF8));
		sprintf(s,"%s",cpu->getAccessResult(i).c_str());
		mainWindow->accessDisplay->SetCellValue(i+1,2,wxString(s,wxConvUTF8));
	}
}

void Presenter::advanceCPU(){
	cpu->exec();
	populateWindow();
}

void Presenter::getInputFile(){
	 wxFileDialog openFileDialog(mainWindow);
	 if (openFileDialog.ShowModal() == wxID_CANCEL)	return;
	 wxString str = openFileDialog.GetPath();
	 mainWindow->inputFileBox->WriteText(str);
}
