#pragma once
#include "Color.h"
#include "Point.h"
#include "CShape.h"

class CEllipse : public CShape
{
public:
	CEllipse(Color color, const Point& center, double width, double height);
	~CEllipse() = default;

	void Draw(ICanvas& canvas) const override;
	Point GetCenter() const;
	double GetWidth() const;
	double GetHeight() const;

private:
	Point m_center;
	double m_width;
	double m_height;
};