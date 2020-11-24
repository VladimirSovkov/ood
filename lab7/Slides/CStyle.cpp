#include "CStyle.h"
#include <stdexcept>

CStyle::CStyle()
{
}

CStyle::CStyle(RGBAColor color)
	:m_isEnabled(true)
	, m_color(color)
{
}

std::optional<bool> CStyle::IsEnabled() const
{
	return m_isEnabled;
}

void CStyle::Enable(bool enable)
{
	m_isEnabled = enable;
}

std::optional<RGBAColor> CStyle::GetColor() const
{
	if (!IsEnabled().value())
	{
		throw std::logic_error("Style is not enabled");
	}
	return m_color;
}

void CStyle::SetColor(RGBAColor color)
{
	m_color = color;
}
