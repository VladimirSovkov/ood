#pragma once
#include <cstdint>

struct Point
{
	double x;
	double y;
};

template <typename T>
struct Rect
{
	Point leftTop;
	T width;
	T height;
};

typedef Rect<double> RectD;

class RGBAColor
{
public:
	RGBAColor(){}
	RGBAColor(float r, float g, float b, float a)
		: r(r)
		, g(g)
		, b(b)
		, a(a)
	{
	}

	float r = 0.f;
	float g = 0.f;
	float b = 0.f;
	float a = 0.f;

	bool operator==(const RGBAColor& color) const
	{
		return (r == color.r) && (g == color.g) && (b == color.b) && (a == color.a);
	}

	bool operator!=(const RGBAColor color) const
	{
		return !(*this == color);
	}
};