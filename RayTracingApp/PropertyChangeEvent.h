#pragma once

#include <wx/wx.h>

#include <any>

#include "Settings.h"

class PropertyChangeEvent : public wxEvent
{
private:
	Settings settings;
	std::any value;

public:
	template<class T>
	PropertyChangeEvent(Settings settings, const T& value);

	wxEvent* Clone() const;

	Settings GetSettings() const;

	template<class T>
	T GetValue() const;
};

wxDECLARE_EVENT(EVT_PROPERTY_CHANGE, PropertyChangeEvent);

template<class T>
inline PropertyChangeEvent::PropertyChangeEvent(Settings settings, const T& value) :
	wxEvent(0, EVT_PROPERTY_CHANGE), settings{ settings }, value{ value }
{
}

template<class T>
inline T PropertyChangeEvent::GetValue() const
{
	return std::any_cast<T>(value);
}

