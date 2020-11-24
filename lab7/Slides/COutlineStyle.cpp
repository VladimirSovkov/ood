#include "COutlineStyle.h"
#include <stdexcept>

COutlineStyle::COutlineStyle()
{
}

COutlineStyle::COutlineStyle(RGBAColor color, double lineSize)
	:m_color(color)
	, m_lineSize(lineSize)
{
}

std::optional<double> COutlineStyle::GetLineThickness() const
{
	if (!IsEnabled().value())
	{
		throw std::logic_error("Outline style is not enabled");
	}
	return m_lineSize;
}

void COutlineStyle::SetLineThickness(double lineSize)
{
	m_lineSize = lineSize;
}

std::optional<bool> COutlineStyle::IsEnabled() const
{
	return m_isEnabled;
}

void COutlineStyle::Enable(bool enable)
{
	m_isEnabled = enable;
}

std::optional<RGBAColor> COutlineStyle::GetColor() const
{
	if (!IsEnabled().value())
	{
		throw std::logic_error("Style is not enabled");
	}
	return m_color;
}

void COutlineStyle::SetColor(RGBAColor color)
{
	m_color = color;
}
