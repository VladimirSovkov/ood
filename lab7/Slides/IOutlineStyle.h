#pragma once
#include <optional>
#include "IStyle.h"

class IOutlineStyle : public IStyle
{
public:
	virtual ~IOutlineStyle() = default;

	virtual std::optional<double> GetLineThickness() const = 0;
	virtual void SetLineThickness(double lineSize) = 0;
};

typedef std::function<void(IOutlineStyle&)> OutlineStyleFunc;