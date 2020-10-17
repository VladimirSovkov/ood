#include "../../catch2/catch.hpp"
#include "MockCanvas.h"
#include "../Shape/CEllipse.h"
#include "../Shape/CRectangle.h"
#include "../Shape/CRegularPolygon.h"
#include "../Shape/CTriangle.h"
#include <sstream>

TEST_CASE("rectangle on canvas")
{
	std::ostringstream ss;
	CMockCanvas canvas(ss);
	CRectangle rectangle(Color::Red, Point{ 0, 2.5 }, Point{ 3.5, 0 });
	rectangle.Draw(canvas);

	std::ostringstream sample;
	sample
		<< "draw line (0.00, 2.50), (3.50, 2.50)\n"
		<< "draw line (0.00, 2.50), (0.00, 0.00)\n"
		<< "draw line (3.50, 2.50), (3.50, 0.00)\n"
		<< "draw line (0.00, 0.00), (3.50, 0.00)\n";

	CHECK(Color::Red == canvas.GetColor());
	CHECK(sample.str() == ss.str());
}

TEST_CASE("triangle on canvas")
{
	std::ostringstream ss;
	CMockCanvas canvas(ss);
	Point vertex1{ 1.5, 3 };
	Point vertex2{ 0, 3 };
	Point vertex3{ 0, 0 };
	CTriangle triangle(Color::Blue, vertex1, vertex2, vertex3);
	triangle.Draw(canvas);

	std::ostringstream sample;
	sample
		<< "draw line (1.50, 3.00), (0.00, 3.00)\n"
		<< "draw line (0.00, 3.00), (0.00, 0.00)\n"
		<< "draw line (0.00, 0.00), (1.50, 3.00)\n";

	CHECK(Color::Blue == canvas.GetColor());
	CHECK(sample.str() == ss.str());
}

TEST_CASE("regular polygon on canvas")
{
	std::ostringstream ss;
	CMockCanvas canvas(ss);
	CRegularPolygon regularPoligon(Color::Pink, 5, {4, 4}, 2);
	regularPoligon.Draw(canvas);

	std::ostringstream sample;
	sample
		<< "draw line (4.00, 6.00), (2.10, 4.62)\n"
		<< "draw line (2.10, 4.62), (2.82, 2.38)\n"
		<< "draw line (2.82, 2.38), (5.18, 2.38)\n"
		<< "draw line (5.18, 2.38), (5.90, 4.62)\n"
		<< "draw line (5.90, 4.62), (4.00, 6.00)\n";
	
	CHECK(Color::Pink == canvas.GetColor());
	CHECK(sample.str() == ss.str());
}

TEST_CASE("ellipse on convas")
{
	std::ostringstream ss;
	CMockCanvas canvas(ss);
	CEllipse ellipse(Color::Red, Point{2, 2}, 2, 1);
	ellipse.Draw(canvas);

	std::ostringstream sample;
	sample << "draw ellipse with center = (2, 2), width = 2, height = 1" << std::endl;

	CHECK(sample.str() == ss.str());
	CHECK(Color::Red == canvas.GetColor());
}