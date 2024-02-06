#include "Range.h"

const Range Range::NonNegative(0.0, std::numeric_limits<double>::max());

Range::Range(double min, double max) :
	min{ min }, max{ max }
{
}

double Range::GetMin() const
{
	return min;
}

double Range::GetMax() const
{
	return max;
}

bool Range::Contains(double value) const
{
	return min <= value && value <= max;
}
