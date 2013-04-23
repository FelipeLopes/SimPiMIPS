#include "main_window.h"

BEGIN_EVENT_TABLE(MainWindow,wxFrame)
	EVT_BUTTON(MainWindow::ID_ADVANCE_BUTTON,MainWindow::onClickAdvance)
	EVT_BUTTON(MainWindow::ID_INPUT_FILE_BROWSE_BUTTON,MainWindow::onClickInputFileBrowse)
	EVT_BUTTON(MainWindow::ID_INST_FILE_BROWSE_BUTTON,MainWindow::onClickInstFileBrowse)
	EVT_BUTTON(MainWindow::ID_INIT_BUTTON,MainWindow::onClickInit)
END_EVENT_TABLE()

MainWindow::MainWindow(Presenter* presenter):
	wxFrame(NULL,-1,_("SimPiMIPS"),wxDefaultPosition,wxSize(600,700)){
	this->presenter = presenter;
	pipelineDisplay = new DisplayGrid(this,wxPoint(0,25),wxSize(405,60),2,5);
	registerDisplay = new DisplayGrid(this,wxPoint(0,150),wxSize(485,210),8,4);
	registerDisplay->SetDefaultColSize(120,true);
	miscDisplay = new DisplayGrid(this,wxPoint(0,375),wxSize(240,110),4,2);
	miscDisplay->SetDefaultColSize(120,true);
	accessDisplay = new DisplayGrid(this,wxPoint(250,375),wxSize(300,135),5,3);
	accessDisplay->SetColSize(0,60);
	accessDisplay->SetColSize(1,120);
	accessDisplay->SetColSize(2,120);
	advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(0,100));
	inputFileBrowseButton = new wxButton(this,ID_INPUT_FILE_BROWSE_BUTTON,_("Browse..."),wxPoint(300,550));
	inputFileBox = new wxTextCtrl(this, ID_INPUT_FILE_BOX,
	      wxEmptyString,wxPoint(0,550), wxSize(300,30));
	instFileBrowseButton = new wxButton(this,ID_INST_FILE_BROWSE_BUTTON,_("Browse..."),wxPoint(300,600));
	instFileBox = new wxTextCtrl(this, ID_INST_FILE_BOX,
	      wxEmptyString,wxPoint(0,600), wxSize(300,30));
	initButton = new wxButton(this,ID_INIT_BUTTON,_("Initialize"),wxPoint(400,550));
}

void MainWindow::onClickAdvance(wxCommandEvent& event){
	presenter->advanceCPU();
}

void MainWindow::onClickInputFileBrowse(wxCommandEvent& event){
	presenter->getFilePath(inputFileBox);
}

void MainWindow::onClickInstFileBrowse(wxCommandEvent& event){
	presenter->getFilePath(instFileBox);
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
