#include "display_grid.h"

DisplayGrid::DisplayGrid(wxWindow* parent, const wxPoint& pos, const wxSize& size,
		int numRows, int numCols):
		wxGrid(parent,wxID_ANY,pos,size){
	CreateGrid(numRows,numCols);
#ifdef __MINGW32__
	for (int i=0; i<numRows; i++) SetRowLabelValue(i,wxEmptyString);
	for (int i=0; i<numCols; i++) SetColLabelValue(i,wxEmptyString);
#endif
	SetColLabelSize(2);
	SetRowLabelSize(2);
	EnableEditing(false);
	EnableDragCell(false);
	EnableDragGridSize(false);
	ClearSelection();
}
