#pragma once

#include <cmath>

struct Color
{
	double r{};
	double g{};
	double b{};

	Color();

	Color(int r, int g, int b);

	Color(double r, double g, double b);

	Color& operator+=(const Color& color);

	Color& operator*=(const Color& color);

	Color& operator/=(double n);

	Color GetGammaCorrected() const;
};

Color operator*(double a, const Color& color);

Color operator*(Color a, const Color& b);

Color operator+(Color a, const Color& b);

Color operator/(Color a, double n);