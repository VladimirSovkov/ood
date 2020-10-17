#pragma once
#include "CShape.h"

class CRectangle : public CShape
{
public:
	CRectangle(Color color, const Point& leftTop, const Point& rightBottom);
	virtual ~CRectangle() = default;

	virtual void Draw(ICanvas& canvas) const override;
	Point GetLeftTop() const;
	Point GetRightBottom() const;

private:
	Point m_leftTop;
	Point m_rightBottom;
};