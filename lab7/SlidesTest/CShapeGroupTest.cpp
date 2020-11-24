#include "../Slides/CShapeGroup.h"
#include "../../catch2/catch.hpp"
#include "../Slides/CRectangle.h"
#include "../Slides/CTriangle.h"
#include "../Slides/CEllipse.h"
#include "MockCanvas.h"
#include <limits>
#include <cmath>

namespace {

	std::shared_ptr<CRectangle> GetRectanglePtr()
	{
		auto rectangle = std::make_shared<CRectangle>(Point{ 0, 0 }, 4., 5.);
		rectangle->GetFillStyle()->SetColor(RGBAColor(1.f, 0, 0, 1.f));
		rectangle->GetOutlineStyle()->SetColor(RGBAColor(1.f, 1.f, 1.f, 1.f));
		rectangle->GetOutlineStyle()->SetLineThickness(0.5);
		return rectangle;
	}

	std::shared_ptr<CEllipse> GetEllipsePtr()
	{
		auto ellipse = std::make_shared<CEllipse>(Point{ 2, 2 }, 2., 1.);
		ellipse->GetFillStyle()->SetColor(RGBAColor(1.f, 0, 0, 1.f));
		ellipse->GetOutlineStyle()->SetColor(RGBAColor(1.f, 1.f, 1.f, 1.f));
		ellipse->GetOutlineStyle()->SetLineThickness(0.5);
		return ellipse;
	}

	std::shared_ptr<CTriangle> GetTrianglePtr()
	{
		auto triangle = std::make_shared<CTriangle>(Point{ 0, 1 }, Point{ 2, 4.3 }, Point{ 5.5, -0.5 });
		triangle->GetFillStyle()->SetColor(RGBAColor(1.f, 0, 0, 1.f));
		triangle->GetOutlineStyle()->SetColor(RGBAColor(1.f, 1.f, 1.f, 1.f));
		triangle->GetOutlineStyle()->SetLineThickness(0.5);
		return triangle;
	}

	bool IsFramesEqual(RectD const& sample, RectD value)
	{
		value.leftTop.x = std::trunc(value.leftTop.x * 10000) / 10000;
		value.leftTop.y = std::trunc(value.leftTop.y * 10000) / 10000;
		value.height = std::trunc(value.height * 10000) / 10000;
		value.width = std::trunc(value.width * 10000) / 10000;

		return std::fabs(sample.leftTop.x - value.leftTop.x) < std::numeric_limits<double>::epsilon()
			&& std::fabs(sample.leftTop.y - value.leftTop.y) < std::numeric_limits<double>::epsilon()
			&& std::fabs(sample.width - value.width) < std::numeric_limits<double>::epsilon()
			&& std::fabs(sample.height - value.height) < std::numeric_limits<double>::epsilon();
	}
}

TEST_CASE("AddShape() adds shape to group")
{
	CShapeGroup shapeGroup;
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetEllipsePtr());
	shapeGroup.AddShape(GetTrianglePtr());

	CHECK(3 == shapeGroup.GetShapesCount());
}

TEST_CASE("RemoveShape() removes a shape from a group by index")
{
	CShapeGroup shapeGroup;
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetTrianglePtr());

	SECTION("correct deletion")
	{
		shapeGroup.RemoveShape(1);
		CHECK(1 == shapeGroup.GetShapesCount());

		shapeGroup.RemoveShape(0);
		CHECK(0 == shapeGroup.GetShapesCount());
	}

	SECTION("index out of range")
	{
		CHECK_THROWS(shapeGroup.RemoveShape(2));
	}
}

TEST_CASE("GetShape() get pointer of shape by index")
{
	CShapeGroup shapeGroup;
	auto rectangle = GetRectanglePtr();
	auto triangle = GetTrianglePtr();

	shapeGroup.AddShape(rectangle);
	shapeGroup.AddShape(triangle);

	SECTION("correct deletion")
	{
		CHECK(rectangle == shapeGroup.GetShape(0));
		CHECK(triangle == shapeGroup.GetShape(1));
	}

	SECTION("index out of range")
	{
		CHECK_THROWS(shapeGroup.GetShape(2));
	}
}

TEST_CASE("GetGroup() get pointer to group")
{
	auto shapeGroupPtr = std::make_shared<CShapeGroup>();
	shapeGroupPtr->AddShape(GetRectanglePtr());
	auto answer = shapeGroupPtr->GetGroup();

	CHECK(shapeGroupPtr == answer);
}
	
TEST_CASE("GetFrame() get the frame of a group of shapes")
{
	auto shapeGroup = CShapeGroup();
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetTrianglePtr());
	shapeGroup.AddShape(GetEllipsePtr());
	RectD sample{ Point {0, -0.5}, 5.5, 5.5 };
	auto answer = shapeGroup.GetFrame();
	CHECK(IsFramesEqual(sample, answer.value()));

	SECTION("get an empty group frame")
	{
		auto shapeGroupEmpty = std::make_shared<CShapeGroup>();
		CHECK(std::nullopt == shapeGroupEmpty->GetFrame());
	}
	
	SECTION("Get the frame of a rectangle")
	{
		auto rectangle = shapeGroup.GetShape(0);
		auto answer = rectangle->GetFrame();
		sample = { Point {0, 0}, 4., 5. };
		CHECK(IsFramesEqual(sample, answer.value()));
	}

	SECTION("Get the frame of a triangle")
	{
		auto triangle = shapeGroup.GetShape(1);
		auto answer = triangle->GetFrame();
		sample = { Point {0, -0.5}, 5.5, 4.8 };
		CHECK(IsFramesEqual(sample, answer.value()));
	}

	SECTION("Get the frame of a triangle")
	{
		auto triangle = shapeGroup.GetShape(2);
		auto answer = triangle->GetFrame();
		sample = { Point{0, 1}, 4, 2 };
		CHECK(IsFramesEqual(sample, answer.value()));
	}

}

TEST_CASE("SetFrame() sets a new frame for the group shape")
{
	auto shapeGroup = CShapeGroup();
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetTrianglePtr());
	shapeGroup.AddShape(GetEllipsePtr());
	auto frame = shapeGroup.GetFrame();
	RectD sample{ Point {1, 1}, 10, 10 };
	shapeGroup.SetFrame(sample);
	auto answer = shapeGroup.GetFrame().value();
	CHECK(IsFramesEqual(sample, answer));

	//rectangle
	auto rectangleFrame = shapeGroup.GetShape(0)->GetFrame();
	sample = { {1., 1.9090}, 7.2727, 9.0909 };
	CHECK(IsFramesEqual(sample, rectangleFrame.value()));
	
	//triangle
	auto triangleFrame = shapeGroup.GetShape(1)->GetFrame();
	sample = { {1., 1.}, 10, 8.7272 };
	CHECK(IsFramesEqual(sample, triangleFrame.value()));

	//ellipse
	auto ellipseFrame = shapeGroup.GetShape(2)->GetFrame();
	sample = { { 1.0, 3.7272 }, 7.2727, 3.6363 };
	CHECK(IsFramesEqual(sample, ellipseFrame.value()));
}

TEST_CASE("GetFillStyle() returns the style of the shapes in the shape group")
{
	CShapeGroup shapeGroup;
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetTrianglePtr());
	shapeGroup.AddShape(GetEllipsePtr());

	SECTION("styles of shapes in group are same")
	{
		CHECK(RGBAColor(1.f, 0, 0, 1.f) == shapeGroup.GetFillStyle()->GetColor().value());
		CHECK(shapeGroup.GetFillStyle()->IsEnabled());
	}
	
	SECTION("different styles of shapes in the group")
	{
		auto rectangle = GetRectanglePtr();
		rectangle->GetFillStyle()->SetColor(RGBAColor(0, 1.f, 1.f, 1.f));
		shapeGroup.RemoveShape(0);
		shapeGroup.AddShape(rectangle);
		CHECK(std::nullopt == shapeGroup.GetFillStyle()->GetColor());
		CHECK(shapeGroup.GetFillStyle()->IsEnabled());

		shapeGroup.GetFillStyle()->SetColor(RGBAColor(0, 1.f, 1.f, 1.f));
		CHECK(RGBAColor(0, 1.f, 1.f, 1.f) == shapeGroup.GetFillStyle()->GetColor());
	}

	SECTION("at least one figure in the group did not have a style")
	{
		shapeGroup.GetFillStyle()->SetColor(RGBAColor(0, 0, 1.f, 1.f));
		auto rectangle = GetRectanglePtr();
		rectangle->GetFillStyle()->Enable(false);
		shapeGroup.RemoveShape(0);
		shapeGroup.AddShape(rectangle);
		CHECK(!shapeGroup.GetFillStyle()->IsEnabled());
		CHECK_THROWS(shapeGroup.GetFillStyle()->GetColor());

		shapeGroup.GetFillStyle()->Enable(true);
		CHECK(std::nullopt == shapeGroup.GetFillStyle()->GetColor());
		CHECK(shapeGroup.GetFillStyle()->IsEnabled());
	}

	SECTION("check the styles for the figurines in the group")
	{
		shapeGroup.GetFillStyle()->SetColor(RGBAColor(0, 1.f, 0, 1.f));

		//rectangle
		auto shape1 = shapeGroup.GetShape(0);
		CHECK(RGBAColor(0, 1.f, 0, 1.f) == shape1->GetFillStyle()->GetColor());
		CHECK(shape1->GetFillStyle()->IsEnabled());

		//triangle
		auto shape2 = shapeGroup.GetShape(1);
		CHECK(RGBAColor(0, 1.f, 0, 1.f) == shape2->GetFillStyle()->GetColor());
		CHECK(shape2->GetFillStyle()->IsEnabled());

		//ellipse
		auto shape3 = shapeGroup.GetShape(2);
		CHECK(RGBAColor(0, 1.f, 0, 1.f) == shape3->GetFillStyle()->GetColor());
		CHECK(shape3->GetFillStyle()->IsEnabled());
	}
}

TEST_CASE("GetOutlineStyle() returns the outline style of the shape group")
{
	CShapeGroup  shapeGroup;
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetTrianglePtr());
	shapeGroup.AddShape(GetEllipsePtr());

	SECTION("style of  outlines of the group shapes is the same")
	{
		auto abc = shapeGroup.GetOutlineStyle()->GetColor();
		CHECK(RGBAColor(1.f, 1.f, 1.f, 1.f) == abc);
		CHECK(shapeGroup.GetOutlineStyle()->IsEnabled());
	}

	SECTION("outline style is different for one group shape")
	{
		THEN("only the color of the outline is different")
		{
			auto rectangle = GetRectanglePtr();
			rectangle->GetOutlineStyle()->SetColor(RGBAColor(0, 1.f, 1.f, 1.f));
			shapeGroup.AddShape(rectangle);
			CHECK(std::nullopt == shapeGroup.GetOutlineStyle()->GetColor());
			CHECK(shapeGroup.GetOutlineStyle()->IsEnabled());
		}

		THEN("only the contour thickness differs")
		{
			auto rectangle = GetRectanglePtr();
			rectangle->GetOutlineStyle()->SetLineThickness(1);
			CHECK(3 == shapeGroup.GetShapesCount());
			shapeGroup.AddShape(rectangle);
			CHECK(std::nullopt == shapeGroup.GetOutlineStyle()->GetLineThickness());
			CHECK(shapeGroup.GetOutlineStyle()->IsEnabled());
		}

		THEN("thickness and color of outline is different")
		{
			auto rectangle = GetRectanglePtr();
			rectangle->GetOutlineStyle()->SetColor(RGBAColor(0, 1.f, 1.f, 1.f));
			rectangle->GetOutlineStyle()->SetLineThickness(1);
			CHECK(3 == shapeGroup.GetShapesCount());
			shapeGroup.AddShape(rectangle);
			CHECK(std::nullopt == shapeGroup.GetOutlineStyle()->GetColor());
			CHECK(std::nullopt == shapeGroup.GetOutlineStyle()->GetLineThickness());
			CHECK(shapeGroup.GetOutlineStyle()->IsEnabled());
		}
	}

	SECTION("one group shape is missing an outline style")
	{
		auto rectangle = GetRectanglePtr();
		rectangle->GetOutlineStyle()->Enable(false);
		shapeGroup.AddShape(rectangle);
		CHECK_FALSE(shapeGroup.GetOutlineStyle()->IsEnabled());
		CHECK_THROWS(shapeGroup.GetOutlineStyle()->GetColor());
		CHECK_THROWS(shapeGroup.GetOutlineStyle()->GetLineThickness());
	}
} 

TEST_CASE("Draw () should draw all the shapes in the group on the canvas")
{
	CShapeGroup  shapeGroup;
	shapeGroup.AddShape(GetRectanglePtr());
	shapeGroup.AddShape(GetTrianglePtr());
	shapeGroup.AddShape(GetEllipsePtr());
	std::ostringstream out;
	MockCanvas canvas(out);
	shapeGroup.Draw(canvas);
	std::ostringstream sample;
	sample
		<< "lineColor: r: 1.0 g: 1.0 b: 1.0 a: 1.0\n"
		<< "lineThickness: 0.5\n"
		<< "line from: 0.0/0.0 to: 4.0/0.0\n"
		<< "line from: 4.0/0.0 to: 4.0/-5.0\n"
		<< "line from: 4.0/-5.0 to: 0.0/-5.0\n"
		<< "line from: 0.0/-5.0 to: 0.0/0.0\n"
		<< "fillColor: r: 1.0 g: 0.0 b: 0.0 a: 1.0\n"
		<< "fillPoligon: 0.0/0.0; 4.0/0.0; 4.0/-5.0; 0.0/-5.0; 4.0/-5.0; \n"
		<< "lineColor: r: 1.0 g: 1.0 b: 1.0 a: 1.0\n"
		<< "lineThickness: 0.5\n"
		<< "line from: 0.0/1.0 to: 2.0/4.3\n"
		<< "line from: 2.0/4.3 to: 5.5/-0.5\n"
		<< "line from: 5.5/-0.5 to: 0.0/1.0\n"
		<< "fillColor: r: 1.0 g: 0.0 b: 0.0 a: 1.0\n"
		<< "fillPoligon: 0.0/1.0; 2.0/4.3; 5.5/-0.5; \n"
		<< "lineColor: r: 1.0 g: 1.0 b: 1.0 a: 1.0\n"
		<< "lineThickness: 0.5\n"
		<< "ellipse center: 2.0/2.0 width: 1 height: 2\n"
		<< "fillColor: r: 1.0 g: 0.0 b: 0.0 a: 1.0\n"
		<< "fillPolygon center: 2.0/2.0 width: 1 height: 2\n";

	CHECK(sample.str() == out.str());
}