#include "SettingsProvider.h"

SettingsProvider SettingsProvider::Instance{};

SettingsProvider::SettingsProvider()
{
	SamplesPerPixel = Property<size_t>(DefaultSettings::SamplesPerPixel.value, [this](size_t value) { EmitEvent(value, Settings::SamplesPerPixel); });
	LookFromX = Property<double>(DefaultSettings::LookFrom.x, [this](double value) { EmitEvent(value, Settings::LookFromX); });
	LookFromY = Property<double>(DefaultSettings::LookFrom.y, [this](double value) { EmitEvent(value, Settings::LookFromY); });
	LookFromZ = Property<double>(DefaultSettings::LookFrom.z, [this](double value) { EmitEvent(value, Settings::LookFromZ); });
}

glm::vec3 SettingsProvider::GetLookFrom() const
{
	return glm::vec3(LookFromX.Get(), LookFromY.Get(), LookFromZ.Get());
}
