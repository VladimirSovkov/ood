#include "../../catch2/catch.hpp"
#include "../ObjectAdapter/CModernGraphicsAdapter.h"
#include "../ObjectAdapter/shape_drawing_lib.h"

TEST_CASE("draw a line in standard (black) color from a standard point")
{
	std::ostringstream out;
	modern_graphics_lib::CModernGraphicsRenderer renderer(out);
	CModernGraphicsAdapter adapter(renderer);

	renderer.BeginDraw();
	adapter.LineTo(1, 1);
	renderer.EndDraw();

	std::ostringstream sample;
	sample << "<draw>\n"
		<< "  <line fromX=\"0\" fromY=\"0\" toX=\"1\" toY=\"1\">\n"
		<< "    <color r=\"0\" g=\"0\" b=\"0\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "</draw>\n";

	CHECK(sample.str() == out.str());
}

TEST_CASE("rendering modern graphics without BeginDraw()")
{
	std::ostringstream out;
	modern_graphics_lib::CModernGraphicsRenderer renderer(out);
	CModernGraphicsAdapter adapter(renderer);
	CHECK_THROWS(adapter.LineTo(1, 1));
	CHECK_THROWS(renderer.EndDraw());
}

TEST_CASE("SetColor sets the color of the line")
{
	std::ostringstream out;
	modern_graphics_lib::CModernGraphicsRenderer renderer(out);
	CModernGraphicsAdapter adapter(renderer);

	renderer.BeginDraw();
	adapter.SetColor(0x660060);
	adapter.LineTo(1, 1);
	renderer.EndDraw();

	std::ostringstream sample;
	sample << "<draw>\n"
		<< "  <line fromX=\"0\" fromY=\"0\" toX=\"1\" toY=\"1\">\n"
		<< "    <color r=\"0.4\" g=\"0\" b=\"0.376471\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "</draw>\n";

	CHECK(sample.str() == out.str());
}

TEST_CASE("Draw shape in Painter")
{
	std::ostringstream out;
	modern_graphics_lib::CModernGraphicsRenderer renderer(out);
	CModernGraphicsAdapter adapter(renderer);
	shape_drawing_lib::CCanvasPainter painter(adapter);

	renderer.BeginDraw();
	shape_drawing_lib::CTriangle triangle({ 0, 0 }, { 3, 5 }, { 6, 0 }, 0x808080);
	shape_drawing_lib::CRectangle rectangle({ 0, 5 }, 6, 4, 0xFFFFFF);
	
	painter.Draw(triangle);
	painter.Draw(rectangle);
	renderer.EndDraw();

	std::ostringstream sample;
	sample << "<draw>\n"
		<< "  <line fromX=\"0\" fromY=\"0\" toX=\"3\" toY=\"5\">\n"
		<< "    <color r=\"0.501961\" g=\"0.501961\" b=\"0.501961\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "  <line fromX=\"3\" fromY=\"5\" toX=\"6\" toY=\"0\">\n"
		<< "    <color r=\"0.501961\" g=\"0.501961\" b=\"0.501961\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "  <line fromX=\"6\" fromY=\"0\" toX=\"0\" toY=\"0\">\n"
		<< "    <color r=\"0.501961\" g=\"0.501961\" b=\"0.501961\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "  <line fromX=\"0\" fromY=\"5\" toX=\"6\" toY=\"5\">\n"
		<< "    <color r=\"1\" g=\"1\" b=\"1\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "  <line fromX=\"6\" fromY=\"5\" toX=\"6\" toY=\"1\">\n"
		<< "    <color r=\"1\" g=\"1\" b=\"1\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "  <line fromX=\"6\" fromY=\"1\" toX=\"0\" toY=\"1\">\n"
		<< "    <color r=\"1\" g=\"1\" b=\"1\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "  <line fromX=\"0\" fromY=\"1\" toX=\"0\" toY=\"5\">\n"
		<< "    <color r=\"1\" g=\"1\" b=\"1\" a=\"1\" />\n"
		<< "  </line>\n"
		<< "</draw>\n";
	CHECK(sample.str() == out.str());
}