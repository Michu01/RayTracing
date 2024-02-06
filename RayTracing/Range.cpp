#include "Range.h"

Range::Range(float min, float max) :
	min{ min }, max{ max }
{
}

bool Range::Contains(float value) const
{
	return min <= value && value <= max;
}
