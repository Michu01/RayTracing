#include "SettingsProvider.h"

SettingsProvider SettingsProvider::Instance{};

SettingsProvider::SettingsProvider()
{
	SamplesPerPixel = Property<size_t>(DefaultSettings::SamplesPerPixel.value, [this](size_t value) { EmitEvent(value, Settings::SamplesPerPixel); });
	LookFromX = Property<double>(DefaultSettings::LookFrom.x, [this](double value) { EmitEvent(value, Settings::LookFromX); });
	LookFromY = Property<double>(DefaultSettings::LookFrom.y, [this](double value) { EmitEvent(value, Settings::LookFromY); });
	LookFromZ = Property<double>(DefaultSettings::LookFrom.z, [this](double value) { EmitEvent(value, Settings::LookFromZ); });
}

Point3d SettingsProvider::GetLookFrom() const
{
	return Point3d(LookFromX.Get(), LookFromY.Get(), LookFromZ.Get());
}
