#pragma once
#include"IStyle.h"

class CStyle : public IStyle
{
public:
	CStyle();
	CStyle(RGBAColor color);

	std::optional<bool> IsEnabled()const override;
	void Enable(bool enable) override;

	std::optional<RGBAColor> GetColor()const override;
	void SetColor(RGBAColor color) override;
private:
	bool m_isEnabled = false;
	std::optional<RGBAColor> m_color = std::nullopt;
};