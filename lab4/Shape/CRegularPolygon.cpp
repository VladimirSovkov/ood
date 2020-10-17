#include "CRegularPolygon.h"
#include "CRegularPolygon.h"
#include <stdexcept>
#include <cmath>
#define PI 3.14159265358979323846

CRegularPolygon::CRegularPolygon(Color color, size_t vertexCount, const Point& center, double radius)
	: CShape(color)
	, m_vertexCount(vertexCount)
	, m_center(center)
	, m_radius(radius)
{
	if (m_vertexCount < 3)
	{
		throw std::invalid_argument("vertex count too smol");
	}
}

void CRegularPolygon::Draw(ICanvas& canvas) const
{
	canvas.SetColor(GetColor());
	std::vector<Point> vertexes = GetAllVertexes();

	for (size_t i = 0; i < m_vertexCount - 1; ++i)
	{
		canvas.DrawLine(vertexes[i], vertexes[i + 1]);
	}
	canvas.DrawLine(vertexes[vertexes.size() - 1], vertexes[0]);
}

size_t CRegularPolygon::GetVertexCount() const
{
	return m_vertexCount;
}

Point CRegularPolygon::GetCenter() const
{
	return m_center;
}

double CRegularPolygon::GetRadius() const
{
	return m_radius;
}

std::vector<Point> CRegularPolygon::GetAllVertexes() const
{
	std::vector<Point> vertexes;
	for (auto i = 0.; i < m_vertexCount; i++)
	{
		Point point{ 0, 0 };
		point.x = m_center.x + m_radius * std::cos(PI / 2 + (2 * PI * i) / m_vertexCount);
		point.y = m_center.y + m_radius * std::sin(PI / 2 + (2 * PI * i) / m_vertexCount);
		vertexes.push_back(point);
	}

	return vertexes;
}
