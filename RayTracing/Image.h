#pragma once

#include <vector>
#include <cassert>
#include <execution>

#include "Size.h"
#include "Color.h"

class Image
{
private:
	SizeU size;
	std::vector<Color> pixels;

public:
	Image();

	Image(SizeU size);

	Image(SizeU size, std::initializer_list<Color> pixels);

	template<class Container>
	Image(SizeU size, const Container& container);

	Color& operator[](size_t n);

	const Color& operator[](size_t n) const;

	const std::vector<Color>& GetPixels() const;

	std::vector<Color>& GetPixels();

	void SetColor(const Color& color);

	size_t GetWidth() const;

	size_t GetHeight() const;

	SizeU GetSize() const;
};

template<class Container>
inline Image::Image(SizeU size, const Container& container) :
	size{ size }, pixels(container.cbegin(), container.cend())
{
}
