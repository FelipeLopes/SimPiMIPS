#include "memory.h"
#include "cpu.h"
#include "app.h"
#include "presenter.h"

IMPLEMENT_APP(App);

bool App::OnInit(){
	Presenter* presenter = new Presenter;
	SetTopWindow(presenter->getMainWindow());
	return true;
}
