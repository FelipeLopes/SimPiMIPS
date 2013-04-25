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
	wxFrame(NULL,wxID_ANY,_("SimPiMIPS"),wxPoint(400,0),wxSize(510,1000)),
	presenter(presenter){

	pipelineDisplay = new DisplayGrid(this,wxPoint(10,210),wxSize(405,60),2,5);
	registerDisplay = new DisplayGrid(this,wxPoint(10,310),wxSize(485,210),8,4);
	registerDisplay->SetDefaultColSize(120,true);
	miscDisplay = new DisplayGrid(this,wxPoint(10,720),wxSize(240,110),4,2);
	miscDisplay->SetDefaultColSize(120,true);
	accessDisplay = new DisplayGrid(this,wxPoint(10,550),wxSize(300,135),5,3);
	accessDisplay->SetColSize(0,60);
	accessDisplay->SetColSize(1,120);
	accessDisplay->SetColSize(2,120);

	instFileBox = new wxTextCtrl(this, wxID_ANY,
			wxEmptyString,wxPoint(120,5), wxSize(300,30));
	inputFileBox = new wxTextCtrl(this, wxID_ANY,
			wxEmptyString,wxPoint(120,45), wxSize(300,30));
	clocksBox = new wxTextCtrl(this,wxID_ANY,
			wxEmptyString,wxPoint(240,145));
	useBypassingCheckBox = new wxCheckBox(this,wxID_ANY,_("Use bypassing"),
			wxPoint(120,90));

	pipelineDisplayLabel = new wxStaticText(this,wxID_ANY,_("Pipeline stages:"),
			wxPoint(10,190));
	registerDisplayLabel = new wxStaticText(this,wxID_ANY,_("Registers:"),
			wxPoint(10,290));
	miscDisplayLabel = new wxStaticText(this,wxID_ANY,_("Miscellaneous:"),
			wxPoint(10,700));
	accessDisplayLabel = new wxStaticText(this,wxID_ANY,_("Recent memory accesses:"),
			wxPoint(10,530));
	instFileLabel = new wxStaticText(this,wxID_ANY,_("Instructions file:"),wxPoint(10,10));
	inputFileLabel = new wxStaticText(this,wxID_ANY,_("Input file:"),wxPoint(55,50));
	clocksLabel = new wxStaticText(this,wxID_ANY,_("Clocks:"),wxPoint(190,150),
			wxSize(150,30));

	nextStepButton = new wxButton(this,ID_NEXT_STEP_BUTTON,_("Next step"),wxPoint(10,140));
	advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(325,140));
	initButton = new wxButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(300,85));
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
