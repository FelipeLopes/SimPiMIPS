#include <cstdio>
#include "main_window.h"
#include "cpu.h"
#include "presenter.h"
#include "file_parser.h"

Presenter::Presenter(){
	cpu = NULL;
	mainWindow = new MainWindow(this);
	mainWindow->init();
	mainWindow->Show(true);
}

MainWindow* Presenter::getMainWindow(){
	return mainWindow;
}

void Presenter::advanceCPU(int n){
	if (cpu==NULL){
		wxMessageBox(_("CPU wasn't initialized!"),_("OMFG Error!"),wxICON_ERROR);
		return;
	}
	for (int i=0; i<n; i++) cpu->exec();
	mainWindow->populateWindow(cpu,instDesc);
	if (cpu->isExecutionFinished()) mainWindow->cpuStatusLabel->SetLabel(_("Execution status:\nFinished"));
	else mainWindow->cpuStatusLabel->SetLabel(_("Execution status:\nExecuting"));
}

wxString Presenter::getFilePath(){
	 wxFileDialog openFileDialog(mainWindow, _("Open file"), _(""), _(""),
			 _("Text files (*.txt)|*.txt"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);;
	 if (openFileDialog.ShowModal() == wxID_CANCEL)	return wxEmptyString;
	 wxString str = openFileDialog.GetPath();
	 return str;
}

void Presenter::initializeCPU(){
	instMem = new Memory;
	dataMem = new Memory;
	reg = new Register;
	int sizeProg = parseInstFile(mainWindow->instFileBox->GetLineText(0),instMem,instDesc);
	if(sizeProg==0){
		wxMessageBox(_("Instruction file doesn't exist or has the wrong format.\n"
						"(Last lines with whitespace?)"),_("OMFG Error!"),wxICON_ERROR);
		return;
	}
	if (!parseInputFile(mainWindow->inputFileBox->GetLineText(0),dataMem)){
		wxMessageBox(_("Input file doesn't exist or has the wrong format.\n"),
					_("OMFG Error!"),wxICON_ERROR);
		return;
	}
	cpu = new CPU(reg,instMem,dataMem,
			mainWindow->useBypassingCheckBox->GetValue(),
			sizeProg);
	mainWindow->populateWindow(cpu,instDesc);
	mainWindow->cpuStatusLabel->SetLabel(_("Execution status:\nCPU ready"));
}

void Presenter::dumpOutput(){
	if (cpu==NULL || !cpu->isExecutionFinished()){
		wxMessageBox(_("CPU hasn't finished executing."),_("OMFG Error!"),wxICON_ERROR);
		return;
	}
	wxString path;
	wxFileDialog saveFileDialog(mainWindow, _("Save file"), _(""), _(""),
			_("Text files (*.txt)|*.txt"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)	return;
	path = saveFileDialog.GetPath();
	FILE* file = fopen(path.mb_str(),"w");
	if (file==NULL){
		wxMessageBox(_("Could not open file for writing."),	_("OMFG Error!"),wxICON_ERROR);
		return;
	}
	for (int i=0; i<1000; i++) fprintf(file,"%d\n",cpu->dataMem->read(5000+4*i));
	fclose(file);
}
