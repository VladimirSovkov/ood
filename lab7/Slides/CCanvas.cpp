#include "CCanvas.h"
#define _USE_MATH_DEFINES
#include <math.h>


CCanvas::CCanvas(GLFWwindow* window)
	: m_window(window)
{
}

void CCanvas::SetFillColor(RGBAColor color)
{
	m_fillColor = color;
}

void CCanvas::SetLineColor(RGBAColor color)
{
	m_outlineColor = color;
}

void CCanvas::SetLineThickness(double thickness)
{
	m_lineThickness = float(thickness);
}

void CCanvas::DrawLine(Point const& from, Point const& to)
{
	glLineWidth(m_lineThickness);
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH);
	glColor4f(m_outlineColor.r, m_outlineColor.g, m_outlineColor.b, m_outlineColor.a);
	glVertex2d(from.x, from.y);
	glVertex2d(to.x, to.y);
	glEnd();
} 

void CCanvas::DrawEllipse(Point const& center, double width, double height)
{
	const int pointCount = 360;
	const float step = float(2 * M_PI / pointCount);
	glBegin(GL_LINE_LOOP);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(m_lineThickness);
	glColor4f(m_outlineColor.r, m_outlineColor.g, m_outlineColor.b, m_outlineColor.a);
	for (float angle = 0; angle < float(2 * M_PI); angle += step)
	{
		const double dx = width * cos(angle);
		const double dy = height * sin(angle);
		glVertex2d(dx + center.x, dy + center.y);
	}
	//for (int i = 0; i < 360; i++)
	//{
	//	double degInRad = i * M_PI / 180;
	//	glVertex2d((cos(degInRad) * horizontalRadius) + center.x, (sin(degInRad) * verticalRadius) + center.y);
	//}
	glEnd();
}

void CCanvas::FilEllipse(Point const& center, double width, double height)
{
	const int pointCount = 360;
	const float step = float(2 * M_PI / pointCount);

	glBegin(GL_POLYGON);
	glColor4f(m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);
	for (float angle = 0; angle < float(2 * M_PI); angle += step)
	{
		const double dx = width * cos(angle);
		const double dy = height * sin(angle);
		glVertex2d(dx + center.x, dy + center.y);
	}
	glEnd();
}

void CCanvas::FillPoligon(std::vector<Point> const& pointsArr)
{
	glColor4f(m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);
	glBegin(GL_POLYGON);
	for (auto& vertex : pointsArr)
	{
		glVertex2d(vertex.x, vertex.y);
	}
	glEnd();
}
