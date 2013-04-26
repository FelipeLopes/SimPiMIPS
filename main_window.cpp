#include <cstdlib>
#include "main_window.h"

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
	presenter(presenter){
#ifdef __MINGW32__
	pipelineDisplay = new DisplayGrid(this,wxPoint(10,210),wxSize(817,52),2,5);
	pipelineDisplay->SetDefaultColSize(163,true);
	pipelineDisplay->SetDefaultRowSize(25,true);
	pipelineDisplay->SetMargins(-10,-10);
	registerDisplay = new DisplayGrid(this,wxPoint(10,300),wxSize(482,202),8,4);
	registerDisplay->SetDefaultColSize(120,true);
	registerDisplay->SetDefaultRowSize(25,true);
	registerDisplay->SetMargins(-10,-10);
	miscDisplay = new DisplayGrid(this,wxPoint(530,300),wxSize(297,102),4,2);
	miscDisplay->SetColSize(0,165);
	miscDisplay->SetColSize(1,130);
	miscDisplay->SetMargins(-10,-10);
	miscDisplay->SetDefaultRowSize(25,true);
	accessDisplay = new DisplayGrid(this,wxPoint(530,440),wxSize(297,127),5,3);
	accessDisplay->SetMargins(-10,-10);
	accessDisplay->SetColSize(0,55);
	accessDisplay->SetColSize(1,120);
	accessDisplay->SetColSize(2,120);
	accessDisplay->SetDefaultRowSize(25,true);

	instFileBox = new wxTextCtrl(this, wxID_ANY,
			wxEmptyString,wxPoint(110,10), wxSize(300,25));
	inputFileBox = new wxTextCtrl(this, wxID_ANY,
			wxEmptyString,wxPoint(110,45), wxSize(300,25));
	clocksBox = new wxTextCtrl(this,wxID_ANY,
			wxEmptyString,wxPoint(322,145), wxSize(75,25));
	useBypassingCheckBox = new wxCheckBox(this,wxID_ANY,_("Use bypassing"),
			wxPoint(599,50));

	pipelineDisplayLabel = new wxStaticText(this,wxID_ANY,_("Pipeline stages:"),
			wxPoint(10,190));
	registerDisplayLabel = new wxStaticText(this,wxID_ANY,_("Registers:"),
			wxPoint(10,280));
	miscDisplayLabel = new wxStaticText(this,wxID_ANY,_("Miscellaneous:"),wxPoint(530,280));
	accessDisplayLabel = new wxStaticText(this,wxID_ANY,_("Recent memory accesses:"),
			wxPoint(530,420));
	instFileLabel = new wxStaticText(this,wxID_ANY,_("Instructions file:"),wxPoint(23,15));
	inputFileLabel = new wxStaticText(this,wxID_ANY,_("Input file:"),wxPoint(55,50));
	clocksLabel = new wxStaticText(this,wxID_ANY,_("Clocks:"),wxPoint(272,150));
	cpuStatusLabel = new wxStaticText(this,wxID_ANY,_("Execution status:\nCPU uninitialized"),
			wxPoint(10,90));

	nextStepButton = new wxButton(this,ID_NEXT_STEP_BUTTON,_("Next step"),wxPoint(10,144));
	advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(407,147));
	initButton = new wxButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(600,15));
	instFileBrowseButton = new wxButton(this,ID_INST_FILE_BROWSE_BUTTON,_("Browse..."),
			wxPoint(420,10),wxSize(80,25));
	inputFileBrowseButton = new wxButton(this,ID_INPUT_FILE_BROWSE_BUTTON,_("Browse..."),
			wxPoint(420,45),wxSize(80,25));
	dumpOutputButton = new wxButton(this,ID_DUMP_OUTPUT_BUTTON,_("Dump output..."),
			wxPoint(600,144));
#else
	pipelineDisplay = new DisplayGrid(this,wxPoint(10,210),wxSize(817,52),2,5);
	pipelineDisplay->SetDefaultColSize(163,true);
	pipelineDisplay->SetMargins(-10,-10);
	registerDisplay = new DisplayGrid(this,wxPoint(10,300),wxSize(482,202),8,4);
	registerDisplay->SetDefaultColSize(120,true);
	registerDisplay->SetMargins(-10,-10);
	miscDisplay = new DisplayGrid(this,wxPoint(530,300),wxSize(297,102),4,2);
	miscDisplay->SetColSize(0,165);
	miscDisplay->SetColSize(1,130);
	miscDisplay->SetMargins(-10,-10);
	accessDisplay = new DisplayGrid(this,wxPoint(530,440),wxSize(297,127),5,3);
	accessDisplay->SetMargins(-10,-10);
	accessDisplay->SetColSize(0,55);
	accessDisplay->SetColSize(1,120);
	accessDisplay->SetColSize(2,120);

	instFileBox = new wxTextCtrl(this, wxID_ANY,
			wxEmptyString,wxPoint(120,5), wxSize(300,30));
	inputFileBox = new wxTextCtrl(this, wxID_ANY,
			wxEmptyString,wxPoint(120,45), wxSize(300,30));
	clocksBox = new wxTextCtrl(this,wxID_ANY,
			wxEmptyString,wxPoint(322,145));
	useBypassingCheckBox = new wxCheckBox(this,wxID_ANY,_("Use bypassing"),
			wxPoint(599,40));

	pipelineDisplayLabel = new wxStaticText(this,wxID_ANY,_("Pipeline stages:"),
			wxPoint(10,190));
	registerDisplayLabel = new wxStaticText(this,wxID_ANY,_("Registers:"),
			wxPoint(10,280));
	miscDisplayLabel = new wxStaticText(this,wxID_ANY,_("Miscellaneous:"),wxPoint(530,280));
	accessDisplayLabel = new wxStaticText(this,wxID_ANY,_("Recent memory accesses:"),
			wxPoint(530,420));
	instFileLabel = new wxStaticText(this,wxID_ANY,_("Instructions file:"),wxPoint(10,10));
	inputFileLabel = new wxStaticText(this,wxID_ANY,_("Input file:"),wxPoint(55,50));
	clocksLabel = new wxStaticText(this,wxID_ANY,_("Clocks:"),wxPoint(272,150),
			wxSize(150,30));
	cpuStatusLabel = new wxStaticText(this,wxID_ANY,_("Execution status:\nCPU uninitialized"),
			wxPoint(10,90));

	nextStepButton = new wxButton(this,ID_NEXT_STEP_BUTTON,_("Next step"),wxPoint(10,144));
	advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(407,144));
	initButton = new wxButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(600,5));
	instFileBrowseButton = new wxButton(this,ID_INST_FILE_BROWSE_BUTTON,_("Browse..."),
			wxPoint(420,5),wxSize(80,30));
	inputFileBrowseButton = new wxButton(this,ID_INPUT_FILE_BROWSE_BUTTON,_("Browse..."),
			wxPoint(420,45),wxSize(80,30));
	dumpOutputButton = new wxButton(this,ID_DUMP_OUTPUT_BUTTON,_("Dump output..."),
			wxPoint(600,144));
#endif
}

void MainWindow::init(){
	pipelineDisplay->SetCellValue(0,0,_("IF"));
	pipelineDisplay->SetCellValue(0,1,_("IDRF"));
	pipelineDisplay->SetCellValue(0,2,_("EX"));
	pipelineDisplay->SetCellValue(0,3,_("DEM"));
	pipelineDisplay->SetCellValue(0,4,_("WB"));

	miscDisplay->SetCellValue(0,0,_("Clock:"));
	miscDisplay->SetCellValue(1,0,_("PC:"));
	miscDisplay->SetCellValue(2,0,_("No. instructions:"));
	miscDisplay->SetCellValue(3,0,_("Productivity:"));

	accessDisplay->SetCellValue(0,0,_("Clock"));
	accessDisplay->SetCellValue(0,1,_("Address"));
	accessDisplay->SetCellValue(0,2,_("Access"));
}

void MainWindow::populateWindow(CPU* cpu, std::vector<std::string>& instDesc){
	for (int i=0; i<5; i++){
		wxString str;
		int state = cpu->getPipelineState(i);
		if (state==Stage::STATE_OUT_OF_PROGRAM) str = _("unknown");
		else if (state==Stage::STATE_STALL) str = _("stall");
		else str = wxString(instDesc[state/4].c_str(),wxConvUTF8);
		pipelineDisplay->SetCellValue(1,i,str);
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
		registerDisplay->SetCellValue(i%8,i/8,wxStr);
	}
	sprintf(s,"%d",cpu->getCurrentClock());
	miscDisplay->SetCellValue(0,1,wxString(s,wxConvUTF8));
	sprintf(s,"%d",cpu->reg->readPC());
	miscDisplay->SetCellValue(1,1,wxString(s,wxConvUTF8));
	sprintf(s,"%d",cpu->getNumInstructions());
	miscDisplay->SetCellValue(2,1,wxString(s,wxConvUTF8));
	if (cpu->getCurrentClock()==0) sprintf(s,"N/A");
	else sprintf(s,"%.2lf%%",100.0*cpu->getNumInstructions()/cpu->getCurrentClock());
	miscDisplay->SetCellValue(3,1,wxString(s,wxConvUTF8));

	for (int i=0; i<cpu->getAccessQueueSize(); i++){
		sprintf(s,"%u",cpu->getAccessClock(i));
		accessDisplay->SetCellValue(i+1,0,wxString(s,wxConvUTF8));
		sprintf(s,"%u",cpu->getAccessAddress(i));
		accessDisplay->SetCellValue(i+1,1,wxString(s,wxConvUTF8));
		sprintf(s,"%s",cpu->getAccessResult(i).c_str());
		accessDisplay->SetCellValue(i+1,2,wxString(s,wxConvUTF8));
}
}

void MainWindow::onClickNextStep(wxCommandEvent& event){
	presenter->advanceCPU(1);
}

void MainWindow::onClickAdvance(wxCommandEvent& event){
	presenter->advanceCPU(atoi(clocksBox->GetValue().mb_str()));
}

void MainWindow::onClickInputFileBrowse(wxCommandEvent& event){
	inputFileBox->ChangeValue(presenter->getFilePath());
}

void MainWindow::onClickInstFileBrowse(wxCommandEvent& event){
	instFileBox->ChangeValue(presenter->getFilePath());
}

void MainWindow::onClickDumpOutput(wxCommandEvent& event){
	presenter->dumpOutput();
}

void MainWindow::onClickInit(wxCommandEvent& event){
	presenter->initializeCPU();
}

MainWindow::~MainWindow(){
	delete pipelineDisplay;
	delete registerDisplay;
	delete miscDisplay;
	delete accessDisplay;
	delete advanceButton;
	delete initButton;
	delete inputFileBrowseButton;
	delete instFileBrowseButton;
	delete presenter;
}
