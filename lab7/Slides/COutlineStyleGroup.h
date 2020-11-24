#pragma once
#include "IOutlineStyle.h"

typedef std::function<void(OutlineStyleFunc)> OutlineStyleEnumerator;

class COutlineStyleGroup : public IOutlineStyle
{
public:
	COutlineStyleGroup(OutlineStyleEnumerator styleEnum);

	virtual std::optional<bool> IsEnabled() const override;
	virtual void Enable(bool enable) override;

	virtual std::optional<RGBAColor> GetColor() const override;
	virtual void SetColor(RGBAColor color) override;

	//line thickness
	virtual std::optional<double> GetLineThickness() const override;
	virtual void SetLineThickness(double lineSize) override;

private:
	OutlineStyleEnumerator m_styleEnum;

};