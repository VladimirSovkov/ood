#pragma once
#include "IStyle.h"

typedef std::function<void(StyleFunc)> StyleEnumerator;

class CStyleGroup : public IStyle
{
public:
	CStyleGroup(StyleEnumerator styleEnum);

	std::optional<bool> IsEnabled()const override;
	void Enable(bool enable) override;

	std::optional<RGBAColor> GetColor()const override;
	void SetColor(RGBAColor color) override;
private:
	StyleEnumerator m_styleEnum;
};