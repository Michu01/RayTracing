#pragma once

#include <wx/wx.h>

#include "MainFrame.h"

class App : public wxApp
{
private:
	const wxSize size{ 1200, 800 };

	MainFrame* frame;

private:
	bool OnInit() override;

public:
	App();
};

wxIMPLEMENT_APP(App);