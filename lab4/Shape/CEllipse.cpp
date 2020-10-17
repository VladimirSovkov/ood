#include "CEllipse.h"

CEllipse::CEllipse(Color color, const Point& center, double width, double height)
	: CShape(color)
	, m_center(center)
	, m_width(width)
	, m_height(height)
{
}
void CEllipse::Draw(ICanvas& canvas) const
{
	canvas.SetColor(GetColor());
	canvas.DrawEllipse(m_center, m_width, m_height);
}

Point CEllipse::GetCenter() const
{
	return m_center;
}

double CEllipse::GetWidth() const
{
	return m_width;
}

double CEllipse::GetHeight() const
{
	return m_height;
}