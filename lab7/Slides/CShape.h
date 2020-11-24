#pragma once
#include "IShape.h"
#include <memory>
#include "IOutlineStyle.h"

class CShape : public IShape
{
public:
	CShape();

	std::shared_ptr<IOutlineStyle> GetOutlineStyle() override;
	virtual std::shared_ptr<const IOutlineStyle> GetOutlineStyle()const override;

	std::shared_ptr<IStyle> GetFillStyle() override;
	std::shared_ptr<const IStyle> GetFillStyle()const override;

	void Draw(ICanvas& canvas) override;

	std::shared_ptr<const IShapeGroup> GetGroup() const override;
	std::shared_ptr<IShapeGroup> GetGroup() override;

protected:
	virtual void DrawLine(ICanvas& canvas) const = 0;
	virtual void DrawFill(ICanvas& canvas) const = 0;

private:
	std::shared_ptr<IStyle> m_fillStyle;
	std::shared_ptr<IOutlineStyle> m_outlineStyle;
};