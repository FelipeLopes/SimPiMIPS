#include "main_window.h"

BEGIN_EVENT_TABLE(MainWindow,wxFrame)
	EVT_BUTTON(MainWindow::ID_ADVANCE_BUTTON,MainWindow::onClickAdvance)
END_EVENT_TABLE()

MainWindow::MainWindow(CPU* cpu):
	wxFrame(NULL,-1,_("SimPiMIPS"),wxDefaultPosition,wxDefaultSize){
	this->cpu=cpu;
	pipelineDisplay = new DisplayGrid(this,wxPoint(0,25),wxSize(425,50),2,5);
	wxButton* advanceButton = new wxButton(this,ID_ADVANCE_BUTTON,_("Advance"),wxPoint(0,100));
	presenter = new Presenter(this,this->cpu);
}

void MainWindow::onClickAdvance(wxCommandEvent& event){
	presenter->advanceCPU();
}

MainWindow::~MainWindow(){
	delete presenter;
}
