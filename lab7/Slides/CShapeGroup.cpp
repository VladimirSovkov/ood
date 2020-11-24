#include "CShapeGroup.h"

CShapeGroup::CShapeGroup()
{
	StyleEnumerator fillStyleEnum = [&](StyleFunc callback) {
		for (auto& shape : m_shapes)
		{
			callback(*shape->GetFillStyle());
		}
	};
	m_fillStyle = std::make_shared<CStyleGroup>(fillStyleEnum);

	OutlineStyleEnumerator outlineStyleEnum = [&](OutlineStyleFunc callback) {
		for (auto& shape : m_shapes)
		{
			callback(*shape->GetOutlineStyle());
		}
	};
	m_outlineStyle = std::make_shared<COutlineStyleGroup>(outlineStyleEnum);
}

void CShapeGroup::Draw(ICanvas& canvas)
{
	for (auto& shape : m_shapes)
	{
		shape->Draw(canvas);
	}
}

std::optional<RectD> CShapeGroup::GetFrame() const
{
	if (m_shapes.empty())
	{
		return std::nullopt;
	}

	double minX;
	double maxX;
	double minY;
	double maxY;
	bool isInit = false;

	for (auto& shape : m_shapes)
	{	
		if (shape->GetFrame())
		{
			auto frame = *shape->GetFrame();
			if (!isInit)
			{
				isInit = true;
				minX = frame.leftTop.x;
				minY = frame.leftTop.y;
				maxX = frame.leftTop.x + frame.width;
				maxY = frame.leftTop.y + frame.height;
			}
			else
			{
				minX = std::min(minX, frame.leftTop.x);
				minY = std::min(minY, frame.leftTop.y);

				maxX = std::max(maxX, frame.leftTop.x + frame.width);
				maxY = std::max(maxY, frame.leftTop.y + frame.height);
			}
		}
	}

	if (isInit)
		return RectD{ {minX, minY}, maxX - minX, maxY - minY };
	else
		return std::nullopt;
}

void CShapeGroup::SetFrame(const RectD& rect)
{
	std::optional<RectD> isOldFrame = GetFrame();
	if (!isOldFrame)
	{
		return;
	}

	RectD oldFrame = *isOldFrame;

	for (auto& shape : m_shapes)
	{
		if (shape->GetFrame())
		{
			auto shapeFrame = *shape->GetFrame();

			double leftTopX = rect.leftTop.x + (shapeFrame.leftTop.x - oldFrame.leftTop.x) / oldFrame.width * rect.width;
			double leftTopY = rect.leftTop.y + (shapeFrame.leftTop.y - oldFrame.leftTop.y) / oldFrame.height * rect.height;
			double width = (shapeFrame.width * rect.width) / oldFrame.width;
			double height = (shapeFrame.height * rect.height) / oldFrame.height;
			shape->SetFrame(RectD{ {leftTopX, leftTopY}, width, height });
		}
	}
}

std::shared_ptr<IOutlineStyle> CShapeGroup::GetOutlineStyle()
{
	return m_outlineStyle;
}

std::shared_ptr<const IOutlineStyle> CShapeGroup::GetOutlineStyle() const
{
	return m_outlineStyle;
}

std::shared_ptr<IStyle> CShapeGroup::GetFillStyle()
{
	return m_fillStyle;
}

std::shared_ptr<const IStyle> CShapeGroup::GetFillStyle() const
{
	return m_fillStyle;
}

std::shared_ptr<const IShapeGroup> CShapeGroup::GetGroup() const
{
	return shared_from_this();
}

std::shared_ptr<IShapeGroup> CShapeGroup::GetGroup()
{
	return shared_from_this();
}

std::shared_ptr<IShape> CShapeGroup::GetShape(size_t index) const
{
	if (index >= m_shapes.size())
	{
		throw std::out_of_range("Index out of range");
	}
	return m_shapes.at(index);
}

size_t CShapeGroup::GetShapesCount() const
{
	return m_shapes.size();
}

void CShapeGroup::AddShape(std::shared_ptr<IShape> shape)
{
	m_shapes.push_back(shape);
}

void CShapeGroup::RemoveShape(size_t index)
{
	if (index >= m_shapes.size())
	{
		throw std::out_of_range("Index out of range");
	}
	m_shapes.erase(m_shapes.begin() + index);
}
