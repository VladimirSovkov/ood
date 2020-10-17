#include "../../catch2/catch.hpp"
#include "../Shape/CEllipse.h"
#include "../Shape/CRectangle.h"
#include "../Shape/CTriangle.h"
#include "../Shape/CRegularPolygon.h"

TEST_CASE("Ellips")
{
	Point center{1.2, 3};
	CEllipse ellipse(Color::Red, center, 3, 4);
	CHECK(center == ellipse.GetCenter());
	CHECK(Color::Red == ellipse.GetColor());
	CHECK(3 == ellipse.GetWidth());
	CHECK(4 == ellipse.GetHeight());
}

TEST_CASE("Rectangle")
{
	Point leftTop{ 0, 0 };
	Point rightBottom{ 3.2, 4.5 };
	CRectangle rectangle(Color::Red, leftTop, rightBottom);

	CHECK(Color::Red == rectangle.GetColor());
	CHECK(leftTop == rectangle.GetLeftTop());
	CHECK(rightBottom == rectangle.GetRightBottom());

}

TEST_CASE("Triangle")
{
	Point vertex1{ 1.5, 3 };
	Point vertex2{ 0, 3 };
	Point vertex3{ 0, 0 };
	CTriangle triangle(Color::Blue, vertex1, vertex2, vertex3);

	CHECK(Color::Blue == triangle.GetColor());
	CHECK(vertex1 == triangle.GetVertex1());
	CHECK(vertex2 == triangle.GetVertex2());
	CHECK(vertex3 == triangle.GetVertex3());
}

TEST_CASE("Regular poligon")
{
	const Point center{ 0, 0 };
	CRegularPolygon regularPoligon(Color::Green, 3, center, 2);
	
	CHECK(Color::Green == regularPoligon.GetColor());
	CHECK(3 == regularPoligon.GetVertexCount());
	CHECK(center == regularPoligon.GetCenter());
	CHECK(2 == regularPoligon.GetRadius());
}