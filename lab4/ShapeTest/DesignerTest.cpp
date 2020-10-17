#include "../../catch2/catch.hpp"
#include "../Shape/CDesigner.h"
#include "../Shape/CShapeFactory.h"
#include <sstream>

TEST_CASE("get a draft without shapes")
{
	CShapeFactory factory;
	CDesigner designer(factory);
	std::stringstream ss;
	auto pictureDraft =	designer.CreateDraft(ss);
	CHECK(0 == pictureDraft.GetShapeCount());
}

TEST_CASE("get a draft with shapes")
{
	std::stringstream ss;
	ss << "rectangle red 3 0 0 4" << std::endl;
	ss << "triangle green 3 0 5 2 3 4" << std::endl;
	CShapeFactory factory;
	CDesigner designer(factory);
	auto pictureDraft = designer.CreateDraft(ss);
	
	SECTION("Get Shape Count")
	{
		CHECK(2 == pictureDraft.GetShapeCount());
	}
	
	SECTION("checking parameters of shapes in drawing")
	{
		CRectangle draftRectangle = dynamic_cast<CRectangle&>(pictureDraft.GetShape(0));
		CHECK(Color::Red == draftRectangle.GetColor());
		CHECK(Point{ 3, 0 } == draftRectangle.GetLeftTop());
		CHECK(Point{ 0, 4 } == draftRectangle.GetRightBottom());

		CTriangle draftTriangle = dynamic_cast<CTriangle&>(pictureDraft.GetShape(1));
		CHECK(Color::Green == draftTriangle.GetColor());
		CHECK(Point{ 3, 0 } == draftTriangle.GetVertex1());
		CHECK(Point{ 5, 2 } == draftTriangle.GetVertex2());
		CHECK(Point{ 3, 4 } == draftTriangle.GetVertex3());
	}
}