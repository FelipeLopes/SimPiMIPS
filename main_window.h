#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/wx.h>
#include "display_grid.h"
#include "presenter.h"

class MainWindow: public wxFrame{
public:
	MainWindow(CPU* cpu);
	DisplayGrid* pipelineDisplay;
	DisplayGrid* registerDisplay;
	virtual ~MainWindow();
protected:
	Presenter* presenter;
	CPU* cpu;
	void onClickAdvance(wxCommandEvent& event);
	enum{
		ID_ADVANCE_BUTTON
	};
	DECLARE_EVENT_TABLE()
};



#endif /* MAIN_WINDOW_H_ */
