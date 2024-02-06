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

	wxStaticText* lookFromLabel = new wxStaticText(cameraPanel, wxID_ANY, "Look from");

	wxPanel* lookFromPanel = new wxPanel(cameraPanel);

	wxSpinCtrlDouble* lookFromXControl = new wxSpinCtrlDouble(lookFromPanel);

	lookFromXControl->SetMinSize(wxSize(70, -1));
	lookFromXControl->SetIncrement(0.01);
	lookFromXControl->SetRange(-100.0, 100.0);
	lookFromXControl->SetValue(DefaultSettings::LookFrom.x);
	lookFromXControl->Bind(wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) { SettingsProvider::Instance.LookFromX.Set(event.GetValue()); });

	wxSpinCtrlDouble* lookFromYControl = new wxSpinCtrlDouble(lookFromPanel);

	lookFromYControl->SetMinSize(wxSize(70, -1));
	lookFromYControl->SetIncrement(0.01);
	lookFromYControl->SetRange(-100.0, 100.0);
	lookFromYControl->SetValue(DefaultSettings::LookFrom.y);
	lookFromYControl->Bind(wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) { SettingsProvider::Instance.LookFromY.Set(event.GetValue()); });

	wxSpinCtrlDouble* lookFromZControl = new wxSpinCtrlDouble(lookFromPanel);

	lookFromZControl->SetMinSize(wxSize(70, -1));
	lookFromZControl->SetIncrement(0.01);
	lookFromZControl->SetRange(-100.0, 100.0);
	lookFromZControl->SetValue(DefaultSettings::LookFrom.z);
	lookFromZControl->Bind(wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) { SettingsProvider::Instance.LookFromZ.Set(event.GetValue()); });

	wxBoxSizer* lookFromSizer = new wxBoxSizer(wxHORIZONTAL);

	lookFromSizer->Add(lookFromXControl);
	lookFromSizer->Add(lookFromYControl);
	lookFromSizer->Add(lookFromZControl);

	lookFromPanel->SetSizer(lookFromSizer);

	wxFlexGridSizer* cameraSizer = new wxFlexGridSizer(2, wxSize(3, 3));

	cameraSizer->Add(lookFromLabel, wxSizerFlags().Proportion(0).CenterVertical());
	cameraSizer->Add(lookFromPanel, wxSizerFlags().Proportion(1).Expand());

	cameraPanel->SetSizer(cameraSizer);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(rendererPanel, wxSizerFlags().Expand().Border(wxBOTTOM, 5));
	mainSizer->Add(cameraPanel, wxSizerFlags().Expand());

	SetSizer(mainSizer);
}
