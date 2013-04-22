#include "display_grid.h"

DisplayGrid::DisplayGrid(wxWindow* parent, const wxPoint& pos, const wxSize& size,
		int numRows, int numCols):
		wxGrid(parent,-1,pos,size){
	CreateGrid(numRows,numCols);
	SetColLabelSize(0);
	SetRowLabelSize(0);
	EnableEditing(false);
	EnableDragCell(false);
	EnableDragGridSize(false);
	ClearSelection();
}
