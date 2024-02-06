#include "Color.h"

Color::Color()
{
}

Color::Color(int r, int g, int b) :
	r{ r / 255.0 }, g{ g / 255.0 }, b{ b / 255.0 }
{
}

Color::Color(double r, double g, double b) :
	r{ r }, g{ g }, b{ b }
{
}

Color& Color::operator+=(const Color& color)
{
	r += color.r;
	g += color.g;
	b += color.b;
	return *this;
}

Color& Color::operator*=(const Color& color)
{
	r *= color.r;
	g *= color.g;
	b *= color.b;
	return *this;
}

Color& Color::operator/=(double n)
{
	double inv = 1.0 / n;
	r *= inv;
	g *= inv;
	b *= inv;
	return *this;
}

Color Color::GetGammaCorrected() const
{
	return { sqrt(r), sqrt(g), sqrt(b) };
}

Color operator*(double a, const Color& color)
{
	return { a * color.r, a * color.g, a * color.b };
}

Color operator*(Color a, const Color& b)
{
	return a *= b;
}

Color operator+(Color a, const Color& b)
{
	return a += b;
}

Color operator/(Color a, double n)
{
	return a /= n;
}
