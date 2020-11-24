#include "CShape.h"
#include "COutlineStyle.h"
#include "ICanvas.h"

CShape::CShape()
{
	m_fillStyle = std::make_shared<CStyle>(RGBAColor());
	m_outlineStyle = std::make_shared<COutlineStyle>();
}

std::shared_ptr<IOutlineStyle> CShape::GetOutlineStyle()
{
	return m_outlineStyle;
}

std::shared_ptr<const IOutlineStyle> CShape::GetOutlineStyle() const
{
	return m_outlineStyle;
}

std::shared_ptr<IStyle> CShape::GetFillStyle()
{
	return m_fillStyle;
}

std::shared_ptr<const IStyle> CShape::GetFillStyle() const
{
	return m_fillStyle;
}

void CShape::Draw(ICanvas& canvas)
{
	if (m_outlineStyle->IsEnabled().value())
	{
		canvas.SetLineColor(*m_outlineStyle->GetColor());
		canvas.SetLineThickness(*m_outlineStyle->GetLineThickness());
		DrawLine(canvas);
	}

	if (m_fillStyle->IsEnabled().value())
	{
		canvas.SetFillColor(m_fillStyle->GetColor().value());
		DrawFill(canvas);
	}
}

std::shared_ptr<const IShapeGroup> CShape::GetGroup() const
{
	return nullptr;
}

std::shared_ptr<IShapeGroup> CShape::GetGroup()
{
	return nullptr;
}