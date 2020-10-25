#include"../../catch2/catch.hpp"
#include "../Shape/CShapeFactory.h"

TEST_CASE("factory should create a rectangle")
{
	CShapeFactory factory;
	auto shape = factory.CreateShape("rectangle red 0 0 3.1 4.5");
	CRectangle& rectangle = dynamic_cast<CRectangle&>(*shape.get());
	CHECK(Color::Red == rectangle.GetColor());
	CHECK(Point{0.0, 0.0} == rectangle.GetLeftTop());
	CHECK(Point{3.1, 4.5} == rectangle.GetRightBottom());
}

TEST_CASE("factory must create a triangle")
{
	CShapeFactory factory;
	auto shape = factory.CreateShape("triangle pink 0 0 3 2.5 4 0");
	CTriangle& triangle = dynamic_cast<CTriangle&>(*shape.get());
	
	CHECK(Color::Pink == triangle.GetColor());
	CHECK(Point{0, 0} == triangle.GetVertex1());
	CHECK(Point{3, 2.5} == triangle.GetVertex2());
	CHECK(Point{ 4, 0 } == triangle.GetVertex3());
}

TEST_CASE("factory should create an ellipse")
{
	CShapeFactory factory;
	auto shape = factory.CreateShape("ellipse Green 3 3 2 3");
	CEllipse& ellipse = dynamic_cast<CEllipse&>(*shape.get());
	
	CHECK(Color::Green == ellipse.GetColor());
	CHECK(Point{3, 3} == ellipse.GetCenter());
	CHECK(2 == ellipse.GetWidth());
	CHECK(3 == ellipse.GetHeight());
}

TEST_CASE("factory should create a regular rectangle")
{
	CShapeFactory factory;
	auto shape = factory.CreateShape("polygon black 6 5 5 3.5");
	CRegularPolygon& poligon = dynamic_cast<CRegularPolygon&>(*shape.get());

	CHECK(Color::Black == poligon.GetColor());
	CHECK(6 == poligon.GetVertexCount());
	CHECK(Point{ 5, 5 } == poligon.GetCenter());
	CHECK(3.5 == poligon.GetRadius());
}

TEST_CASE("factory cannot create shape from invalid string")
{
	CShapeFactory factory;
	CHECK_THROWS(factory.CreateShape(""));
	CHECK_THROWS(factory.CreateShape("shape red 0 0 0 0"));
	CHECK_THROWS(factory.CreateShape("rectangle color 2 0 0 2"));
	CHECK_THROWS(factory.CreateShape("tringle red 0 0 3 2.5 0"));
	CHECK_THROWS(factory.CreateShape("tringle 0 0 3 2.5 0 4 red"));
	CHECK_THROWS(factory.CreateShape("rectangle red 2 0 0 2 2"));
}