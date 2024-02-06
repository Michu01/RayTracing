#pragma once

#include <wx/wx.h>

#include "Property.h"
#include "Settings.h"
#include "PropertyChangeEvent.h"
#include "MinMaxDefaultValue.h"
#include "DefaultSettings.h"

class SettingsProvider : public wxEvtHandler
{
public:
	static SettingsProvider Instance;

public:
	Property<size_t> SamplesPerPixel;
	Property<double> LookFromX;
	Property<double> LookFromY;
	Property<double> LookFromZ;

private:
	SettingsProvider();

	template<class T>
	void EmitEvent(const T& value, Settings settings);

public:
	Point3d GetLookFrom() const;
};

template<class T>
inline void SettingsProvider::EmitEvent(const T& value, Settings settings)
{
	PropertyChangeEvent event(settings, value);

	wxPostEvent(this, event);
}