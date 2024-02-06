#pragma once

#include <wx/wx.h>

#include "Size.h"

inline SizeU wxSizeToSizeU(const wxSize& size)
{
	return SizeU(size.x, size.y);
}