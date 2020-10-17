#include "CCanvas.h"
#define _USE_MATH_DEFINES
#include <math.h>


CCanvas::CCanvas(GLFWwindow* window)
	:m_window(window)
	, m_color(RGB(0, 0, 0))
{
}

void CCanvas::SetColor(Color color)
{
	if (color == Color::Black)
		m_color = RGB(0, 0, 0);
	else if(color == Color::Blue)
	{
		m_color = RGB(0, 0, 1);
	}
	else if (color == Color::Green)
	{
		m_color = RGB(0, 1, 0);
	}
	else if (color == Color::Pink)
	{
		m_color = RGB(1, 0.560f, 0.917f);
	}
	else if (color == Color::Red)
	{
		m_color = RGB(1, 0, 0);
	}
	else if (color == Color::Yellow)
	{
		m_color = RGB(1, 1, 0);
	}
}

void CCanvas::DrawLine(const Point& from, const Point& to)
{
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(m_color.red, m_color.green, m_color.blue);
	glVertex2d(from.x, from.y);
	glVertex2d(to.x, to.y);
	glEnd();
	glEnable(GL_LINE_SMOOTH);
}

void CCanvas::DrawEllipse(const Point& center, double width, double height)
{
	const int pointCount = 360;
	const float step = float(2 * M_PI / pointCount);
	glBegin(GL_LINE_STRIP);
	glColor3f(m_color.red, m_color.green, m_color.blue);
	for (float angle = 0; angle < float(2 * M_PI); angle += step)
	{
		const double dx = width * cos(angle);
		const double dy = height * sin(angle);
		glVertex2d(dx + center.x, dy + center.y);
	}
	glEnd();
}
