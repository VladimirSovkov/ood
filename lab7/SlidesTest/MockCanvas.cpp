#include "MockCanvas.h"

MockCanvas::MockCanvas(std::ostream& out)
	:m_out(out)
{
}

std::string ColorToString(RGBAColor color)
{
	std::stringstream ss;
	ss.precision(1);
	ss << std::fixed
		<< "r: " << color.r
		<< " g: " << color.g
		<< " b: " << color.b
		<< " a: " << color.a;
	return ss.str();
}

void MockCanvas::SetFillColor(RGBAColor color)
{
	m_out << "fillColor: " << ColorToString(color) << std::endl;
}

void MockCanvas::SetLineColor(RGBAColor color)
{
	m_out << "lineColor: " << ColorToString(color) << std::endl;
}

void MockCanvas::SetLineThickness(double thickness)
{
	m_out << "lineThickness: " << thickness << std::endl;
}

std::string PointToString(Point const& point)
{
	std::stringstream ss;
	ss.precision(1);
	ss << std::fixed << point.x << "/" << point.y;
	return ss.str();
}

void MockCanvas::DrawLine(Point const& from, Point const& to)
{
	m_out << "line from: " << PointToString(from) << " to: " << PointToString(to) << std::endl;
}

void MockCanvas::DrawEllipse(Point const& center, double width, double height)
{
	m_out << "ellipse center: " << PointToString(center) 
		<< " width: " << width 
		<< " height: " << height << std::endl;
}

void MockCanvas::FilEllipse(Point const& center, double width, double height)
{
	m_out << "fillPolygon center: " << PointToString(center) 
		<< " width: " << width 
		<< " height: " << height << std::endl;
}

void MockCanvas::FillPoligon(std::vector<Point> const& pointsArr)
{
	m_out << "fillPoligon: ";
	for (auto const& point: pointsArr)
	{
		m_out << PointToString(point)<< "; ";
	}
	m_out << std::endl;
}
