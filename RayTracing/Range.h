#pragma once

#include <limits>

class Range
{
private:
	double min{};
	double max{};

public:
	static const Range NonNegative;

public:
	Range(double min, double max);

	double GetMin() const;

	double GetMax() const;

	bool Contains(double value) const;
};
