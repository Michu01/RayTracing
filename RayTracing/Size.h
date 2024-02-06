#pragma once

template<class T>
struct Size
{
	T width{};
	T height{};
};

using SizeU = Size<unsigned>;
using SizeD = Size<double>;

template<class T>
double GetAspectRatio(Size<T> size);

template<class T>
inline double GetAspectRatio(Size<T> size)
{
	return (double)size.width / size.height;
}
