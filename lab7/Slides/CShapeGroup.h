#pragma once
#include "IShapeGroup.h"
#include <vector>
#include "CStyleGroup.h"
#include "COutlineStyleGroup.h"
#include <stdexcept>

class CShapeGroup : 
	public IShapeGroup
	, public std::enable_shared_from_this<CShapeGroup>
{
public:
	CShapeGroup();

	void Draw( ICanvas& canvas) override;

	std::optional<RectD> GetFrame() const;
	void SetFrame(const RectD& rect);

	std::shared_ptr<IOutlineStyle> GetOutlineStyle();
	std::shared_ptr<const IOutlineStyle> GetOutlineStyle() const;

	std::shared_ptr<IStyle> GetFillStyle();
	std::shared_ptr<const IStyle> GetFillStyle() const;

	std::shared_ptr<const IShapeGroup> GetGroup() const override;
	std::shared_ptr<IShapeGroup> GetGroup() override;

	std::shared_ptr<IShape> GetShape(size_t index) const override;
	size_t GetShapesCount() const override;

	void AddShape(std::shared_ptr<IShape> shape) override;
	void RemoveShape(size_t index) override;

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
	std::shared_ptr<IStyle>  m_fillStyle;
	std::shared_ptr<IOutlineStyle> m_outlineStyle;
};