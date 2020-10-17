#pragma once
#include "CShape.h"
#include "Point.h"
#include <vector>

class CRegularPolygon : public CShape
{
public:

	CRegularPolygon(Color color, size_t vertexCount, const Point& center, double radius);
	~CRegularPolygon() = default;

	void Draw(ICanvas& canvas) const override;

	size_t GetVertexCount() const;
	Point GetCenter() const;
	double GetRadius() const;


private:
	std::vector<Point> GetAllVertexes() const;

	size_t m_vertexCount;
	Point m_center;
	double m_radius;
};