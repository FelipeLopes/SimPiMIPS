#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/wx.h>
#include "display_grid.h"
#include "presenter.h"

class MainWindow: public wxFrame{
public:
	MainWindow(Presenter* presenter);
	void init();
	void populateWindow(CPU* cpu,std::vector<std::string>& instDesc);
	DisplayGrid *pipelineDisplay,*registerDisplay,*miscDisplay,*accessDisplay;
	wxTextCtrl *inputFileBox,*instFileBox,*clocksBox;
	wxStaticText *cpuStatusLabel;
	wxCheckBox *useBypassingCheckBox;
	virtual ~MainWindow();
protected:
	Presenter* presenter;
	wxButton *nextStepButton,*advanceButton,*inputFileBrowseButton,
			 *instFileBrowseButton,*initButton,*dumpOutputButton;
	wxStaticText *pipelineDisplayLabel,*registerDisplayLabel,*miscDisplayLabel,
				 *accessDisplayLabel,*inputFileLabel,*instFileLabel,*clocksLabel;
	void onClickNextStep(wxCommandEvent& event);
	void onClickAdvance(wxCommandEvent& event);
	void onClickInputFileBrowse(wxCommandEvent& event);
	void onClickInstFileBrowse(wxCommandEvent& event);
	void onClickDumpOutput(wxCommandEvent& event);
	void onClickInit(wxCommandEvent& event);
	enum{
		ID_INPUT_FILE_BROWSE_BUTTON = wxID_HIGHEST + 1,
		ID_INST_FILE_BROWSE_BUTTON,
		ID_INIT_BUTTON,
		ID_NEXT_STEP_BUTTON,
		ID_PLAY_PAUSE_BUTTON,
		ID_ADVANCE_BUTTON,
		ID_DUMP_OUTPUT_BUTTON
	};
	DECLARE_EVENT_TABLE()
};



#endif /* MAIN_WINDOW_H_ */
