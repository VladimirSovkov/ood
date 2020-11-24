#pragma once
#include "IOutlineStyle.h"

class COutlineStyle : public IOutlineStyle
{
public:
	COutlineStyle();
	COutlineStyle(RGBAColor color, double lineSize);

	std::optional<double> GetLineThickness() const override;
	void SetLineThickness(double lineSize) override;

	std::optional<bool> IsEnabled() const override;
	void Enable(bool enable) override;

	std::optional<RGBAColor> GetColor() const override;
	void SetColor(RGBAColor color) override;

private:
	bool m_isEnabled = true;
	std::optional<RGBAColor> m_color = std::nullopt;
	std::optional<double> m_lineSize = std::nullopt;
};