#pragma once
#include "CShape.h"

class CEllipse : public CShape
{
public:
	CEllipse(Point center, double width, double height);

	Point GetCenter() const;
	double GetHorizontalRadius() const;
	double GetVerticalRadius() const;

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

protected:
	void DrawLine(ICanvas& canvas) const override;
	void DrawFill(ICanvas& canvas) const override;

private:
	Point m_center;
	double m_horizontalRadius;
	double m_verticalRadius;
};