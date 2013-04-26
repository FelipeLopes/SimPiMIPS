#include "main_window.h"

BEGIN_EVENT_TABLE(MainWindow,wxFrame)
	EVT_BUTTON(MainWindow::ID_NEXT_STEP_BUTTON,MainWindow::onClickNextStep)
	EVT_BUTTON(MainWindow::ID_ADVANCE_BUTTON,MainWindow::onClickAdvance)
	EVT_BUTTON(MainWindow::ID_INPUT_FILE_BROWSE_BUTTON,MainWindow::onClickInputFileBrowse)
	EVT_BUTTON(MainWindow::ID_INST_FILE_BROWSE_BUTTON,MainWindow::onClickInstFileBrowse)
	EVT_BUTTON(MainWindow::ID_INIT_BUTTON,MainWindow::onClickInit)
	EVT_BUTTON(MainWindow::ID_DUMP_OUTPUT_BUTTON,MainWindow::onClickDumpOutput)
	EVT_CHAR(MainWindow::onClickNextStep)
END_EVENT_TABLE()

MainWindow::MainWindow(Presenter* presenter):
	wxFrame(NULL,wxID_ANY,_("SimPiMIPS"),wxPoint(400,0),wxSize(840,630)),
	presenter(presenter){

	pipelineDisplay = new DisplayGrid(this,wxPoint(10,210),wxSize(482,52),2,5);
	pipelineDisplay->SetDefaultColSize(96,true);
	pipelineDisplay->SetMargins(0,-10);
	registerDisplay = new DisplayGrid(this,wxPoint(10,300),wxSize(482,202),8,4);
	registerDisplay->SetDefaultColSize(120,true);
	registerDisplay->SetMargins(0,-10);
	miscDisplay = new DisplayGrid(this,wxPoint(530,210),wxSize(302,102),4,2);
	miscDisplay->SetColSize(0,170);
	miscDisplay->SetColSize(1,130);
	miscDisplay->SetMargins(0,-10);
	accessDisplay = new DisplayGrid(this,wxPoint(530,375),wxSize(302,127),5,3);
	accessDisplay->SetMargins(0,-10);
	accessDisplay->SetColSize(0,60);
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
	miscDisplayLabel = new wxStaticText(this,wxID_ANY,_("Miscellaneous:"),
			wxPoint(530,190));
	accessDisplayLabel = new wxStaticText(this,wxID_ANY,_("Recent memory accesses:"),
			wxPoint(530,355));
	instFileLabel = new wxStaticText(this,wxID_ANY,_("Instructions file:"),wxPoint(10,10));
	inputFileLabel = new wxStaticText(this,wxID_ANY,_("Input file:"),wxPoint(55,50));
	clocksLabel = new wxStaticText(this,wxID_ANY,_("Clocks:"),wxPoint(272,150),
			wxSize(150,30));

	nextStepButton = new wxButton(this,ID_NEXT_STEP_BUTTON,_("Next step"),wxPoint(10,144));
	advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(407,144));
	initButton = new wxButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(600,5));
	instFileBrowseButton = new wxButton(this,ID_INST_FILE_BROWSE_BUTTON,_("Browse..."),
			wxPoint(420,5),wxSize(80,30));
	inputFileBrowseButton = new wxButton(this,ID_INPUT_FILE_BROWSE_BUTTON,_("Browse..."),
			wxPoint(420,45),wxSize(80,30));
	dumpOutputButton = new wxButton(this,wxID_ANY,_("Dump output..."),
			wxPoint(360,550));
}

void MainWindow::onClickNextStep(wxCommandEvent& event){
	presenter->advanceCPU(1);
}

void MainWindow::onClickAdvance(wxCommandEvent& event){

}

void MainWindow::onClickInputFileBrowse(wxCommandEvent& event){
	presenter->getFilePath(inputFileBox);
}

void MainWindow::onClickInstFileBrowse(wxCommandEvent& event){
	presenter->getFilePath(instFileBox);
}

void MainWindow::onClickDumpOutput(wxCommandEvent& event){

}

void MainWindow::onClickInit(wxCommandEvent& event){
	presenter->init();
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
