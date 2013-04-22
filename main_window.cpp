#include "main_window.h"

BEGIN_EVENT_TABLE(MainWindow,wxFrame)
	EVT_BUTTON(MainWindow::ID_ADVANCE_BUTTON,MainWindow::onClickAdvance)
END_EVENT_TABLE()

MainWindow::MainWindow(CPU* cpu):
	wxFrame(NULL,-1,_("SimPiMIPS"),wxDefaultPosition,wxSize(600,600)){
	this->cpu=cpu;
	pipelineDisplay = new DisplayGrid(this,wxPoint(0,25),wxSize(405,60),2,5);
	registerDisplay = new DisplayGrid(this,wxPoint(0,150),wxSize(485,210),8,4);
	registerDisplay->SetDefaultColSize(120,true);
	miscDisplay = new DisplayGrid(this,wxPoint(0,375),wxSize(240,110),4,2);
	miscDisplay->SetDefaultColSize(120,true);
	accessDisplay = new DisplayGrid(this,wxPoint(250,375),wxSize(300,135),5,3);
	accessDisplay->SetColSize(0,60);
	accessDisplay->SetColSize(1,120);
	accessDisplay->SetColSize(2,120);
	wxButton* advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(0,100));
	presenter = new Presenter(this,this->cpu);
}

void MainWindow::onClickAdvance(wxCommandEvent& event){
	presenter->advanceCPU();
}

MainWindow::~MainWindow(){
	delete presenter;
}
