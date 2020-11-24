#include "CRectangle.h"

CRectangle::CRectangle(Point leftTop, double width, double height)
	:m_leftTop(leftTop)
	, m_width(width)
	, m_height(height)
	, CShape()
{
}

Point CRectangle::GetLeftTop() const
{
	return m_leftTop;
}

double CRectangle::GetWidth() const
{
	return m_width;
}

double CRectangle::GetHeight() const
{
	return m_height;
}

std::optional<RectD> CRectangle::GetFrame() const
{
	return RectD({ m_leftTop, m_width, m_height});
}

void CRectangle::SetFrame(const RectD& rect)
{
	m_leftTop = rect.leftTop;
	m_height = rect.height;
	m_width = rect.width;
}

void CRectangle::DrawLine(ICanvas& canvas) const
{
	Point rightBottom = { m_leftTop.x + m_width, m_leftTop.y - m_height };
	canvas.DrawLine(m_leftTop, { m_leftTop.x + m_width, m_leftTop.y });
	canvas.DrawLine({ m_leftTop.x + m_width, m_leftTop.y }, rightBottom);
	canvas.DrawLine(rightBottom, { m_leftTop.x, m_leftTop.y - m_height });
	canvas.DrawLine({ m_leftTop.x, m_leftTop.y - m_height }, m_leftTop);
}

void CRectangle::DrawFill(ICanvas& canvas) const
{
	Point rightBottom = { m_leftTop.x + m_width, m_leftTop.y - m_height };
	std::vector<Point> pointArr = {
		m_leftTop,
		{ m_leftTop.x + m_width, m_leftTop.y },
		rightBottom,
		{ m_leftTop.x, m_leftTop.y - m_height },
		rightBottom
	};
	canvas.FillPoligon(pointArr);
}
