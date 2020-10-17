#pragma once
#include "../Shape/ICanvas.h"
#include <ostream>
#include <set>
#include <iomanip>

class CMockCanvas : public ICanvas
{
public:
	CMockCanvas(std::ostream& stream)
		:m_stream(stream)
	{
	}

	void SetColor(Color color) override
	{
		m_color = color;
	}

	void DrawLine(const Point& from, const Point& to) override
	{
		m_stream << std::fixed << std::setprecision(2)
			<< "draw line (" << from.x << ", " << from.y << "), ("
			<< to.x << ", " << to.y << ")" << std::endl;
	}

	void DrawEllipse(const Point& center, double width, double height) override
	{
		m_stream << "draw ellipse with center = ("<< center.x << ", " << center.y << ")" 
			<< ", width = " << width << ", height = " << height << std::endl;
	} 

	Color GetColor()
	{
		return m_color;
	}

private:
	Color m_color = Color::Black;
	std::ostream& m_stream;
};
