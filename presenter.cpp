#include <cstdio>
#include "main_window.h"
#include "cpu.h"
#include "presenter.h"

int teste[] = {
		0b00100000000000100000000000000010, // LI R2,2 ; A
		0b00100000000001010000001111101000, // LI R5, 1000 ; X
		0b00100000000001100001001110001000, // LI R6, 5000 ; Y
		0b00100000000000010000000000000000, // LI R1, 0 ; I
		0b00100000000001110000001111101000, // LI R7, 1000
											// Loop 1000 vezes
		0b10001100101011110000000000000000, // LW R15, 0(R5)
		0b00000000010011110111100000011000, // MUL R15, R15, R2
		0b10001100110100000000000000000000, // LW R16, 0(R6)
		0b00000001111100001000000000100000, // ADD R16, R15, R16
		//0b00000001111100001000000000100010, // SUB R16, R15, R16
		0b10101100110100000000000000000000, // SW R16, 0(R6)
		0b00100000001000010000000000000001, // ADDI R1, R1, 1
		0b00100000101001010000000000000100, // ADDI R5, R5, 4
		0b00100000110001100000000000000100, // ADDI R6, R6, 4
		0b00011100001001110000000000010100  // BLE R1, R7, Loop
		//0b00010000001001111111111111011100  // BNE R1, R7, Loop
};

int x[1000],y[1000];

Presenter::Presenter(){
	cpu = NULL;
	mainWindow = new MainWindow(this);

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

	mainWindow->Show(true);
}

MainWindow* Presenter::getMainWindow(){
	return mainWindow;
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

void Presenter::getFilePath(wxTextCtrl* textCtrl){
	 wxFileDialog openFileDialog(mainWindow);
	 if (openFileDialog.ShowModal() == wxID_CANCEL)	return;
	 wxString str = openFileDialog.GetPath();
	 textCtrl->ChangeValue(str);
}

void Presenter::init(){
	instMem = new Memory;
	dataMem = new Memory;
	reg = new Register;
	cpu = new CPU(reg,instMem,dataMem,true);
	for (int i=0; i<14; i++) instMem->write(i*4,teste[i]);
	reg->writePC(0);
	FILE* file = fopen(mainWindow->inputFileBox->GetLineText(0).mb_str(),"r");
	if (file==NULL){
	printf("Could not open file");
	exit(1);
	}

	for (int i=0; i<1000; i++){
	fscanf(file," %d %d",&x[i],&y[i]);
	}

	for (int i=0; i<1000; i++){
	dataMem->write(1000+4*i,x[i]);
	dataMem->write(5000+4*i,y[i]);
	}
	fclose(file);
}
