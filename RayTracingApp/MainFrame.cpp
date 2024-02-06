#include "MainFrame.h"

MainFrame::MainFrame(const wxSize& size) :
	wxFrame(nullptr, wxID_ANY, "Ray Tracing", wxDefaultPosition, size)
{
	SetFont(GetFont().Scale(1.2f));
	SetBackgroundColour(*wxWHITE);

	SettingsPanel* panel = new SettingsPanel(this);

	panel->SetMinSize(wxSize(280, 0));
	
	RenderView* renderView = new RenderView(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(renderView, wxSizerFlags().Expand().Proportion(6));
	sizer->Add(panel, wxSizerFlags().Expand().Proportion(1).Border(wxALL, 3));

	SetSizer(sizer);
}
