#include "CModernGraphicsAdapter.h"

CModernGraphicsAdapter::CModernGraphicsAdapter(std::ostream& out)
	: CModernGraphicsRenderer(out)
	, m_color(0, 0, 0, 1)
	, m_point(0, 0)
{}

void CModernGraphicsAdapter::SetColor(uint32_t rgbColor)
{
	float r = ((rgbColor >> 16) & 0xff) / 255.0f;
	float g = ((rgbColor >> 8) & 0xff) / 255.0f;
	float b = ((rgbColor) & 0xff) / 255.0f;

	m_color = modern_graphics_lib::CRGBAColor(r, g, b, 1.);
}

void CModernGraphicsAdapter::MoveTo(int x, int y)
{
	m_point = {x, y};
}

void CModernGraphicsAdapter::LineTo(int x, int y)
{
	modern_graphics_lib::CPoint endPoint(x, y);
	DrawLine(m_point, endPoint, m_color);
}