#include "CRectangle.h"

CRectangle::CRectangle(Color color, const Point& leftTop, const Point& rightBottom)
	: CShape(color)
	, m_leftTop(leftTop)
	, m_rightBottom(rightBottom)
{
}

void CRectangle::Draw(ICanvas& canvas) const
{
	Point rightTop = { m_rightBottom.x, m_leftTop.y };
	Point leftBottom = { m_leftTop.x, m_rightBottom.y };

	canvas.SetColor(GetColor());
	canvas.DrawLine(m_leftTop, rightTop);
	canvas.DrawLine(m_leftTop, leftBottom);
	canvas.DrawLine(rightTop, m_rightBottom);
	canvas.DrawLine(leftBottom, m_rightBottom);
}

Point CRectangle::GetLeftTop() const
{
	return m_leftTop;
}

Point CRectangle::GetRightBottom() const
{
	return m_rightBottom;
}