#pragma once
#include"CShape.h"

class CTriangle : public CShape
{
public:
	CTriangle(Point vertex1, Point vertex2, Point vertex3);
	
	Point GetVertex1() const;
	Point GetVertex2() const;
	Point GetVertex3() const;

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

protected:
	void DrawLine(ICanvas& canvas) const override;
	void DrawFill(ICanvas& canvas) const override;

private:
	Point m_vertex1;
	Point m_vertex2;
	Point m_vertex3;
};