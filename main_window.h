#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/wx.h>
#include "display_grid.h"
#include "presenter.h"

class MainWindow: public wxFrame{
public:
	MainWindow(CPU* cpu);
	DisplayGrid *pipelineDisplay,*registerDisplay,*miscDisplay,*accessDisplay;
	wxTextCtrl* inputFileBox;
	virtual ~MainWindow();
protected:
	Presenter* presenter;
	CPU* cpu;
	wxButton *advanceButton,*inputFileBrowseButton;
	void onClickAdvance(wxCommandEvent& event);
	void onClickInputFileBrowse(wxCommandEvent& event);
	enum{
		ID_INPUT_FILE_BOX = wxID_HIGHEST + 1,
		ID_INPUT_FILE_BROWSE_BUTTON,
		ID_ADVANCE_BUTTON
	};
	DECLARE_EVENT_TABLE()
};



#endif /* MAIN_WINDOW_H_ */
