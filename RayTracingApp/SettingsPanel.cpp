#include "SettingsPanel.h"

SettingsPanel::SettingsPanel(wxWindow* parent) : 
	wxPanel(parent)
{
	wxPanel* rendererPanel = new wxPanel(this);

	wxStaticText* samplesPerPixelLabel = new wxStaticText(rendererPanel, wxID_ANY, "Samples");

	wxSpinCtrl* samplesPerPixelControl = new wxSpinCtrl(rendererPanel);

	samplesPerPixelControl->SetMin(DefaultSettings::SamplesPerPixel.min);
	samplesPerPixelControl->SetMax(DefaultSettings::SamplesPerPixel.max);
	samplesPerPixelControl->SetValue(DefaultSettings::SamplesPerPixel.value);
	samplesPerPixelControl->Bind(wxEVT_SPINCTRL, [this](wxSpinEvent& event) { SettingsProvider::Instance.SamplesPerPixel.Set(event.GetValue()); });

	wxFlexGridSizer* rendererSizer = new wxFlexGridSizer(2, wxSize(3, 3));

	rendererSizer->Add(samplesPerPixelLabel, wxSizerFlags().Proportion(0).CenterVertical());
	rendererSizer->Add(samplesPerPixelControl, wxSizerFlags().Proportion(1).Expand());

	rendererPanel->SetSizer(rendererSizer);

	wxPanel* cameraPanel = new wxPanel(this);

	wxFlexGridSizer* cameraSizer = new wxFlexGridSizer(2, wxSize(3, 3));

	cameraPanel->SetSizer(cameraSizer);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(rendererPanel, wxSizerFlags().Expand().Border(wxBOTTOM, 5));
	mainSizer->Add(cameraPanel, wxSizerFlags().Expand());

	SetSizer(mainSizer);
}
