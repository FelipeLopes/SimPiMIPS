#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/event.h>
#include "stage.h"
// ta comparando inteiro sinalizado dar cast pra tirar o
/*
int teste[] = {
		0b00100000000000100000000000000010, // LI R2,2 ; A
		0b00100000000001010000001111101000, // LI R5, 1000 ; X
		0b00100000000001100001001110001000, // LI R6, 5000 ; Y
		0b00100000000000010000000000000000, // LI R1, 0 ; I
		0b00100000000001110000001111101000, // LI R7, 1000
											// Loop 1000 vezes
		0b10001100101011110000000000000000, // LW R15, 0(R5)
		0b00000000010011110111100000011000, // MUL R15, R15, R2
		0b10001100110100000000000000000000, // LW R16, 0(R6)
		0b00000001111100001000000000100000, // ADD R16, R15, R16
		//0b00000001111100001000000000100010, // SUB R16, R15, R16
		0b10101100110100000000000000000000, // SW R16, 0(R6)
		0b00100000001000010000000000000001, // ADDI R1, R1, 1
		0b00100000101001010000000000000100, // ADDI R5, R5, 4
		0b00100000110001100000000000000100, // ADDI R6, R6, 4
		0b00011100001001110000000000010100  // BLE R1, R7, Loop
		//0b00010000001001111111111111011100  // BNE R1, R7, Loop
};

int v[] = {
		0b00100000000010100000000001100100, //; I1: addi R10,R0,100
		0b10101100000000000000000000011000, //; I2: sw R0,24(R0)
		0b10101100000000000000000000011100, //; I3: sw R0,28(R0)
		0b10001100000001100000000000011100, //; I4: lw R6,28(R0)
		0b00000000110001100011100000011000, //; I5: mul R7,R6,R6
		0b10001100000000010000000000011000, //; I6: lw R1,24(R0)
		0b00000000001001110100100000100000, //; I7: add R9,R1,R7
		0b10101100000010010000000000011000, //; I8: sw R9,24(R0)
		0b00100000110001100000000000000001, //; I9: addi R6,R6,1
		0b10101100000001100000000000011100, //; I10: sw R6,28(R0)
		0b00011100110010100000000000001100  //; I11: ble R6,R10,12
};

class App: public wxApp
{
    virtual bool OnInit();
};

class PipelineGrid: public wxGrid{
public:
	PipelineGrid(wxWindow* parent, wxWindowID id,
			const wxPoint& pos,
			const wxSize& size,
			int style = wxWANTS_CHARS,
			const wxString& name = wxPanelNameStr);
	void func(wxGridEvent& event);
};

void PipelineGrid::func(wxGridEvent& event){
	assert(0);
	wxMessageBox( _("This is a wxWidgets Hello world sample"),
	                  _("About Hello World"),
	                  wxOK | wxICON_INFORMATION, NULL);
}


class MainWindow: public wxFrame
{
public:

    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

enum
{
    ID_QUIT = 1,
    ID_ABOUT,
    ID_CELL_SELECTED
};

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_QUIT, MainWindow::OnQuit)
    EVT_MENU(ID_ABOUT, MainWindow::OnAbout)
END_EVENT_TABLE()

IMPLEMENT_APP(App)

bool App::OnInit()
{
    MainWindow *frame = new MainWindow( _("Hello World"), wxPoint(50, 50),
                                  wxSize(450,340) );
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, -1, title, pos, size )
{
	wxGrid* grid = new wxGrid(this,
	                          -1,
	                          wxPoint(0,0),
	                          wxSize(400,300));
		grid->GetGridWindow()->Connect(wxID_ANY,wxEVT_GRID_SELECT_CELL,PipelineGrid::func(wxGridEvent& evt),NULL,grid);
	    // Then we call CreateGrid to set the dimensions of the grid
	    // (100 rows and 10 columns in this example)
	    grid->CreateGrid( 100, 10 );

	    // We can set the sizes of individual rows and columns
	    // in pixels
	    grid->SetRowSize( 0, 60 );
	    grid->SetColSize( 0, 120 );

	    // And set grid cell contents as strings
	    grid->SetCellValue( 0, 0, _("wxGrid is good") );

	    // We can specify that some cells are read-only
	    grid->SetCellValue( 0, 3, _("This is read-only") );
	    grid->SetReadOnly( 0, 3 );

	    // Colours can be specified for grid cell contents
	    grid->SetCellValue(3, 3, _("green on grey"));
	    grid->SetCellTextColour(3, 3, *wxGREEN);
	    grid->SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);

	    // We can specify the some cells will store numeric
	    // values rather than strings. Here we set grid column 5
	    // to hold floating point values displayed with width of 6
	    // and precision of 2
	    grid->SetColFormatFloat(5, 6, 2);
	    grid->SetCellValue(0, 6, _("3.1415"));

    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_ABOUT, _("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_QUIT, _("E&xit") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _("&File") );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _("Welcome to wxWidgets!") );
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("This is a wxWidgets Hello world sample"),
                  _("About Hello World"),
                  wxOK | wxICON_INFORMATION, this);
}
*/

// Version using dynamic event routing
// Use '''Connect''' instead of event table.
// Robert Roebling, Martin Bernreuther, Lance Zhang, Ryan Norton
/*
#include <wx/wx.h>

class MyApp : public wxApp
{
   virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

class MyFrame : public wxFrame
{
public:
   MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
protected:
   // Do we really need to expose the implementation detail? I guess not.
   void OnQuit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);

   void func(wxGridEvent& event){
   	assert(0);
   }
private:
   enum {ID_Quit=wxID_HIGHEST + 1, ID_About, ID_GRID};
};

bool MyApp::OnInit()
{
   wxFrame *frame = new MyFrame(wxT("Hello World"), wxPoint(50,50),
     wxSize(450,350));
     frame->Show(TRUE);
     SetTopWindow(frame);
     return TRUE;
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
	wxGrid* grid = new wxGrid(this,
		                          -1,
		                          wxPoint(0,0),
		                          wxSize(400,300));
		    // Then we call CreateGrid to set the dimensions of the grid
		    // (100 rows and 10 columns in this example)
		    grid->CreateGrid( 100, 10 );

		    // We can set the sizes of individual rows and columns
		    // in pixels
		    grid->SetRowSize( 0, 60 );
		    grid->SetColSize( 0, 120 );

		    // And set grid cell contents as strings
		    grid->SetCellValue( 0, 0, _("wxGrid is good") );

		    // We can specify that some cells are read-only
		    grid->SetCellValue( 0, 3, _("This is read-only") );
		    grid->SetReadOnly( 0, 3 );

		    // Colours can be specified for grid cell contents
		    grid->SetCellValue(3, 3, _("green on grey"));
		    grid->SetCellTextColour(3, 3, *wxGREEN);
		    grid->SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);

		    // We can specify the some cells will store numeric
		    // values rather than strings. Here we set grid column 5
		    // to hold floating point values displayed with width of 6
		    // and precision of 2
		    grid->SetColFormatFloat(5, 6, 2);
		    grid->SetCellValue(0, 6, _("3.1415"));



  // create menubar
  wxMenuBar* menuBar = new wxMenuBar;
  // create menu
  wxMenu* menuFile = new wxMenu;
  // append menu entries
  menuFile->Append(ID_About, wxT("&About..."));
  menuFile->AppendSeparator();
  menuFile->Append(ID_Quit, wxT("E&xit"));
  // append menu to menubar
  menuBar->Append(menuFile, wxT("&File"));
  // set frame menubar
  SetMenuBar(menuBar);

  // connect event handlers
  grid->Connect(wxEVT_GRID_SELECT_CELL,(wxObjectEventFunction)&MyFrame::func);
  Connect(ID_Quit,wxEVT_COMMAND_MENU_SELECTED,
     wxCommandEventHandler(MyFrame::OnQuit));
  Connect(ID_About,wxEVT_COMMAND_MENU_SELECTED,
     wxCommandEventHandler(MyFrame::OnAbout));

  CreateStatusBar();
  SetStatusText(wxT("Welcome to wxWidgets"));
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxT("wxWidgets Hello World example."),wxT("About Hello World"),
        wxOK|wxICON_INFORMATION, this);
}
*/
