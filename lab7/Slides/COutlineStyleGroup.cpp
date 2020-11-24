#include "COutlineStyleGroup.h"

COutlineStyleGroup::COutlineStyleGroup(OutlineStyleEnumerator styleEnum)
	: m_styleEnum(styleEnum)
{
}

std::optional<bool> COutlineStyleGroup::IsEnabled() const
{
	std::optional<bool> isEnabled;
	bool isInit = false;
	auto callback = [&](IOutlineStyle& style) {
		if (!isInit)
		{
			isEnabled = style.IsEnabled();
			isInit = true;
		}
		else if (isEnabled != style.IsEnabled())
		{
			isEnabled = std::nullopt;
		}
	};
	m_styleEnum(callback);
	return isEnabled;
}

void COutlineStyleGroup::Enable(bool enable)
{
	auto callback = [&](IOutlineStyle& style) {
		style.Enable(enable);
	};
	m_styleEnum(callback);
}

std::optional<RGBAColor> COutlineStyleGroup::GetColor() const
{
	std::optional<RGBAColor> color;
	bool isInit = false;
	auto callback = [&](IOutlineStyle& style) {
		if (!isInit)
		{
			color = style.GetColor();
			isInit = true;
		}
		else if (color != style.GetColor())
		{
			color = std::nullopt;
		}
	};
	m_styleEnum(callback);
	return color;
}

void COutlineStyleGroup::SetColor(RGBAColor color)
{
	auto callback = [&](IOutlineStyle& style) {
		style.SetColor(color);
	};
	m_styleEnum(callback);
}

std::optional<double> COutlineStyleGroup::GetLineThickness() const
{
	std::optional<double> LineSize;
	bool isInit = false;
	auto callback = [&](IOutlineStyle& style) {
		if (!isInit)
		{
			LineSize = style.GetLineThickness();
			isInit = true;
		}
		else if (LineSize != style.GetLineThickness())
		{
			LineSize = std::nullopt;
		}
	};
	m_styleEnum(callback);
	return LineSize;
}

void COutlineStyleGroup::SetLineThickness(double lineSize)
{
	auto callback = [&](IOutlineStyle& style) {
		style.SetLineThickness(lineSize);
	};
	m_styleEnum(callback);
}
