#include "../../catch2/catch.hpp"
#include "../Shape/CPictureDraft.h"
#include "../Shape/CTriangle.h"
#include "../Shape/CEllipse.h"
#include "../Shape/CRectangle.h"
#include "../Shape/CRegularPolygon.h"

TEST_CASE("creating a draft without shapes")
{
	std::vector<std::shared_ptr<CShape>> listShape;
	CPictureDraft draft(listShape);

	CHECK(0 == draft.GetShapeCount());
	CHECK_THROWS(draft.GetShape(0));
}

TEST_CASE("creating draft of paintings with transfer of shape")
{
	CEllipse ellipse(Color::Yellow, Point{ 3, 1.5 }, 1, 1);
	CRegularPolygon poligon(Color::Red, 5, Point{ 4, 7 }, 1);
	
	std::shared_ptr<CShape> shape1 = std::make_shared<CEllipse>(ellipse);
	std::shared_ptr<CShape> shape2 = std::make_shared<CRegularPolygon>(poligon);
	
	std::vector<std::shared_ptr<CShape>> listShape{ shape1, shape2 };
	CPictureDraft draft(listShape);

	SECTION("check number of shapes in picture draft")
	{
		CHECK(2 == draft.GetShapeCount());
	}

	SECTION("check the correctness of the returned sketches with the original")
	{
		CEllipse draftEllipse = dynamic_cast<CEllipse&>(draft.GetShape(0));
		CRegularPolygon draftPoligon = dynamic_cast<CRegularPolygon&>(draft.GetShape(1));

		CHECK(ellipse.GetColor() == draftEllipse.GetColor());
		CHECK(ellipse.GetCenter() == draftEllipse.GetCenter());
		CHECK(ellipse.GetHeight() == draftEllipse.GetHeight());
		CHECK(ellipse.GetWidth() == draftEllipse.GetWidth());

		CHECK(poligon.GetCenter() == draftPoligon.GetCenter());
		CHECK(poligon.GetColor() == draftPoligon.GetColor());
		CHECK(poligon.GetRadius() == draftPoligon.GetRadius());
		CHECK(poligon.GetVertexCount() == draftPoligon.GetVertexCount());
	}
}