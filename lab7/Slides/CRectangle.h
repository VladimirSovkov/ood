#pragma once
#include"CShape.h"

class CRectangle : public CShape
{
public:
	CRectangle(Point leftTop, double width, double length);

	Point GetLeftTop() const;
	double GetWidth() const;
	double GetHeight() const;

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

protected:
	void DrawLine(ICanvas& canvas) const override;
	void DrawFill(ICanvas& canvas) const override;
private:
	Point m_leftTop;
	double m_width;
	double m_height;
};