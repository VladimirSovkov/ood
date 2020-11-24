#pragma once
#include "CStyle.h"
#include "IDrawable.h"
#include "IOutlineStyle.h"

class IShapeGroup;

class IShape : public IDrawable
{
public:
	virtual std::optional<RectD> GetFrame() const = 0;
	virtual void SetFrame(const RectD& rect) = 0;

	virtual std::shared_ptr<IOutlineStyle> GetOutlineStyle() = 0;
	virtual std::shared_ptr<const IOutlineStyle> GetOutlineStyle()const = 0;

	virtual std::shared_ptr<IStyle> GetFillStyle() = 0;
	virtual std::shared_ptr<const IStyle> GetFillStyle()const = 0;

	virtual std::shared_ptr<IShapeGroup> GetGroup() = 0;
	virtual std::shared_ptr<const IShapeGroup> GetGroup() const = 0;

	virtual void Draw(ICanvas& canvas) = 0;

	virtual ~IShape() = default;
};