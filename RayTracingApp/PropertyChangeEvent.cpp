#include "PropertyChangeEvent.h"

wxDEFINE_EVENT(EVT_PROPERTY_CHANGE, PropertyChangeEvent);

wxEvent* PropertyChangeEvent::Clone() const
{
    return new PropertyChangeEvent(*this);
}

Settings PropertyChangeEvent::GetSettings() const
{
    return settings;
}
