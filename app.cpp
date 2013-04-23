#include <cstdio>

#include "memory.h"
#include "cpu.h"
#include "app.h"
#include "main_window.h"

IMPLEMENT_APP(App);

bool App::OnInit(){
	Presenter* presenter = new Presenter;
	SetTopWindow(presenter->getMainWindow());
	return true;
}
