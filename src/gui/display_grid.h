#ifndef DISPLAY_GRID_H_
#define DISPLAY_GRID_H_

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/event.h>

class DisplayGrid: public wxGrid{
public:
	DisplayGrid(wxWindow* parent, const wxPoint& pos, const wxSize& size,
	             int numRows, int numCols);
};


#endif /* DISPLAY_GRID_H_ */
