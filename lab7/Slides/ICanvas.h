#pragma once
#include "CommonTypes.h"
#include <vector>

class ICanvas
{
public:
	virtual void SetFillColor(RGBAColor color) = 0;
	virtual void SetLineColor(RGBAColor color) = 0;
	virtual void SetLineThickness(double thickness) = 0;
	virtual void DrawLine(Point const& from, Point const& to) = 0;
	virtual void DrawEllipse(Point const& center, double width, double height) = 0;
	virtual void FilEllipse(Point const& center, double width, double height) = 0;
	virtual void FillPoligon(std::vector<Point> const& pointsArr) = 0;

	virtual ~ICanvas() = default;
};