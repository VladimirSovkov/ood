#pragma once
#include <sstream>
#include "../Slides/ICanvas.h"

class MockCanvas : public ICanvas
{
public:
public:
	MockCanvas(std::ostream& out);

	void SetFillColor(RGBAColor color) override;
	void SetLineColor(RGBAColor color) override;
	void SetLineThickness(double thickness) override;
	void DrawLine(Point const& from, Point const& to) override;
	void DrawEllipse(Point const& center, double width, double height) override;
	void FilEllipse(Point const& center, double width, double height) override;
	void FillPoligon(std::vector<Point> const& pointsArr) override;	
private:
	std::ostream& m_out;
};