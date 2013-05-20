#include <cstdlib>
#include "main_window.h"
#include "presenter.h"

BEGIN_EVENT_TABLE(MainWindow,wxFrame)
	EVT_BUTTON(MainWindow::ID_NEXT_STEP_BUTTON,MainWindow::onClickNextStep)
	EVT_BUTTON(MainWindow::ID_ADVANCE_BUTTON,MainWindow::onClickAdvance)
	EVT_BUTTON(MainWindow::ID_INPUT_FILE_BROWSE_BUTTON,MainWindow::onClickInputFileBrowse)
	EVT_BUTTON(MainWindow::ID_INST_FILE_BROWSE_BUTTON,MainWindow::onClickInstFileBrowse)
	EVT_BUTTON(MainWindow::ID_INIT_BUTTON,MainWindow::onClickInit)
	EVT_BUTTON(MainWindow::ID_DUMP_OUTPUT_BUTTON,MainWindow::onClickDumpOutput)
END_EVENT_TABLE()

MainWindow::MainWindow(Presenter* presenter):
#ifdef __MINGW32__
	wxFrame(NULL,wxID_ANY,_("SimPiMIPS"),wxPoint(400,0),wxSize(855,630)),
#else
	wxFrame(NULL,wxID_ANY,_("SimPiMIPS"),wxPoint(400,0),wxSize(840,630)),
#endif
	presenter(presenter),
	pipelineDisplay(this,wxPoint(10,210),wxSize(817,52),2,5),
	registerDisplay(this,wxPoint(10,300),wxSize(482,202),8,4),
	miscDisplay(this,wxPoint(530,300),wxSize(297,102),4,2),
	accessDisplay(this,wxPoint(530,440),wxSize(297,127),5,3),
	pipelineDisplayLabel(this,wxID_ANY,_("Pipeline stages:"),wxPoint(10,190)),
	registerDisplayLabel(this,wxID_ANY,_("Registers:"),wxPoint(10,280)),
	miscDisplayLabel(this,wxID_ANY,_("Miscellaneous:"),wxPoint(530,280)),
	accessDisplayLabel(this,wxID_ANY,_("Recent memory accesses:"),wxPoint(530,420)),
	inputFileLabel(this,wxID_ANY,_("Input file:"),wxPoint(55,50)),
	cpuStatusLabel(this,wxID_ANY,_("Execution status:\nCPU uninitialized"),wxPoint(10,90)),
	nextStepButton(this,ID_NEXT_STEP_BUTTON,_("Next step"),wxPoint(10,144)),
	dumpOutputButton(this,ID_DUMP_OUTPUT_BUTTON,_("Dump output..."),wxPoint(600,144)),
#ifdef __MINGW32__
	instFileBox(this,wxID_ANY,wxEmptyString,wxPoint(110,10),wxSize(300,25)),
	inputFileBox(this,wxID_ANY,wxEmptyString,wxPoint(110,45),wxSize(300,25)),
	clocksBox(this,wxID_ANY,wxEmptyString,wxPoint(322,145),wxSize(75,25)),
	useBypassingCheckBox(this,wxID_ANY,_("Use bypassing"),wxPoint(599,50)),
	instFileLabel(this,wxID_ANY,_("Instructions file:"),wxPoint(23,15)),
	clocksLabel(this,wxID_ANY,_("Clocks:"),wxPoint(272,150)),
	advanceButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(407,147)),
	initButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(600,15)),
	instFileBrowseButton(this,ID_INST_FILE_BROWSE_BUTTON,_("Browse..."),wxPoint(420,10),wxSize(80,25)),
	inputFileBrowseButton(this,ID_INPUT_FILE_BROWSE_BUTTON,_("Browse..."),wxPoint(420,45),wxSize(80,25))
#else
    instFileBox(this,wxID_ANY,wxEmptyString,wxPoint(120,5),wxSize(300,30)),
	inputFileBox(this,wxID_ANY,wxEmptyString,wxPoint(120,45),wxSize(300,30)),
	clocksBox(this,wxID_ANY,wxEmptyString,wxPoint(322,145)),
	useBypassingCheckBox(this,wxID_ANY,_("Use bypassing"),wxPoint(599,40)),
	instFileLabel(this,wxID_ANY,_("Instructions file:"),wxPoint(10,10)),
	clocksLabel(this,wxID_ANY,_("Clocks:"),wxPoint(272,150),wxSize(150,30)),
	advanceButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(407,144)),
	initButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(600,5)),
	instFileBrowseButton(this,ID_INST_FILE_BROWSE_BUTTON,_("Browse..."),wxPoint(420,5),wxSize(80,30)),
	inputFileBrowseButton(this,ID_INPUT_FILE_BROWSE_BUTTON,_("Browse..."),wxPoint(420,45),wxSize(80,30))
#endif
{
	pipelineDisplay.SetDefaultColSize(163,true);
	pipelineDisplay.SetMargins(-10,-10);
	
	registerDisplay.SetDefaultColSize(120,true);
	registerDisplay.SetMargins(-10,-10);
	
	miscDisplay.SetColSize(0,165);
	miscDisplay.SetColSize(1,130);
	miscDisplay.SetMargins(-10,-10);
	
	accessDisplay.SetMargins(-10,-10);
	accessDisplay.SetColSize(0,55);
	accessDisplay.SetColSize(1,120);
	accessDisplay.SetColSize(2,120);
#ifdef __MINGW32__
	pipelineDisplay.SetDefaultRowSize(25,true);
	registerDisplay.SetDefaultRowSize(25,true);
	miscDisplay.SetDefaultRowSize(25,true);
	accessDisplay.SetDefaultRowSize(25,true);
#endif    
	pipelineDisplay.SetCellValue(0,0,_("IF"));
	pipelineDisplay.SetCellValue(0,1,_("IDRF"));
	pipelineDisplay.SetCellValue(0,2,_("EX"));
	pipelineDisplay.SetCellValue(0,3,_("DEM"));
	pipelineDisplay.SetCellValue(0,4,_("WB"));

	miscDisplay.SetCellValue(0,0,_("Clock:"));
	miscDisplay.SetCellValue(1,0,_("PC:"));
	miscDisplay.SetCellValue(2,0,_("No. instructions:"));
	miscDisplay.SetCellValue(3,0,_("Productivity:"));

	accessDisplay.SetCellValue(0,0,_("Clock"));
	accessDisplay.SetCellValue(0,1,_("Address"));
	accessDisplay.SetCellValue(0,2,_("Access"));
}

MainWindow::~MainWindow(){
	delete presenter;
}

void MainWindow::populateWindow(CPU* cpu, std::vector<std::string>& instDesc){
	for (int i=0; i<5; i++){
		wxString str;
		int state = cpu->getPipelineState(i);
		if (state==Stage::STATE_OUT_OF_PROGRAM) str = _("unknown");
		else if (state==Stage::STATE_STALL) str = _("stall");
		else str = wxString(instDesc[state/4].c_str(),wxConvUTF8);
		pipelineDisplay.SetCellValue(1,i,str);
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
		registerDisplay.SetCellValue(i%8,i/8,wxStr);
	}
	sprintf(s,"%d",cpu->getCurrentClock());
	miscDisplay.SetCellValue(0,1,wxString(s,wxConvUTF8));
	sprintf(s,"%d",cpu->reg->readPC());
	miscDisplay.SetCellValue(1,1,wxString(s,wxConvUTF8));
	sprintf(s,"%d",cpu->getNumInstructions());
	miscDisplay.SetCellValue(2,1,wxString(s,wxConvUTF8));
	if (cpu->getCurrentClock()==0) sprintf(s,"N/A");
	else sprintf(s,"%.2lf%%",100.0*cpu->getNumInstructions()/cpu->getCurrentClock());
	miscDisplay.SetCellValue(3,1,wxString(s,wxConvUTF8));

	for (int i=0; i<cpu->getAccessQueueSize(); i++){
		sprintf(s,"%u",cpu->getAccessClock(i));
		accessDisplay.SetCellValue(i+1,0,wxString(s,wxConvUTF8));
		sprintf(s,"%u",cpu->getAccessAddress(i));
		accessDisplay.SetCellValue(i+1,1,wxString(s,wxConvUTF8));
		sprintf(s,"%s",cpu->getAccessResult(i).c_str());
		accessDisplay.SetCellValue(i+1,2,wxString(s,wxConvUTF8));
    }
}

void MainWindow::onClickNextStep(wxCommandEvent& event){
	presenter->advanceCPU(1);
}

void MainWindow::onClickAdvance(wxCommandEvent& event){
	presenter->advanceCPU(atoi(clocksBox.GetValue().mb_str()));
}

void MainWindow::onClickInputFileBrowse(wxCommandEvent& event){
	inputFileBox.ChangeValue(presenter->getFilePath());
}

void MainWindow::onClickInstFileBrowse(wxCommandEvent& event){
	instFileBox.ChangeValue(presenter->getFilePath());
}

void MainWindow::onClickDumpOutput(wxCommandEvent& event){
	presenter->dumpOutput();
}

void MainWindow::onClickInit(wxCommandEvent& event){
	presenter->initializeCPU();
}
