#include "../../catch2/catch.hpp"
#include "../Shape/CPainter.h"
#include "../Shape/CShapeFactory.h"
#include "../Shape/CDesigner.h"
#include "MockCanvas.h"
#include <sstream>

TEST_CASE("painter paints on canvas")
{
	CShapeFactory factory;
	CDesigner designer(factory);
	std::stringstream ss("triangle red 0 0 1.5 3 3 0\nellipse green 4 4 2 1");
	CPictureDraft draft = designer.CreateDraft(ss);
	std::ostringstream oss;
	CMockCanvas canvas(oss);
	CPainter painter;
	
	painter.DrawPicture(draft, canvas);
	
	std::ostringstream sample;
	sample
		<< "draw line (0.00, 0.00), (1.50, 3.00)\n"
		<< "draw line (1.50, 3.00), (3.00, 0.00)\n"
		<< "draw line (3.00, 0.00), (0.00, 0.00)\n"
		<< "draw ellipse with center = (4.00, 4.00), width = 2.00, height = 1.00" << std::endl;

	CHECK(sample.str() == oss.str());
}