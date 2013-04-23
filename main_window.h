#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/wx.h>
#include "display_grid.h"
#include "presenter.h"

class MainWindow: public wxFrame{
public:
	MainWindow(Presenter* presenter);
	DisplayGrid *pipelineDisplay,*registerDisplay,*miscDisplay,*accessDisplay;
	wxTextCtrl *inputFileBox,*instFileBox;
	virtual ~MainWindow();
protected:
	Presenter* presenter;
	CPU* cpu;
	wxButton *advanceButton,*inputFileBrowseButton,*instFileBrowseButton,*initButton;
	void onClickAdvance(wxCommandEvent& event);
	void onClickInputFileBrowse(wxCommandEvent& event);
	void onClickInstFileBrowse(wxCommandEvent& event);
	void onClickInit(wxCommandEvent& event);
	enum{
		ID_INPUT_FILE_BOX = wxID_HIGHEST + 1,
		ID_INPUT_FILE_BROWSE_BUTTON,
		ID_INST_FILE_BOX,
		ID_INST_FILE_BROWSE_BUTTON,
		ID_INIT_BUTTON,
		ID_ADVANCE_BUTTON
	};
	DECLARE_EVENT_TABLE()
};



#endif /* MAIN_WINDOW_H_ */
