#pragma once
#include "ICanvas.h"
#include <glfw3.h>

class CCanvas : public ICanvas
{
public:
	CCanvas(GLFWwindow* window);

	void SetFillColor(RGBAColor color) override;
	void SetLineColor(RGBAColor color) override;
	void SetLineThickness(double thickness) override;
	void DrawLine(Point const& from, Point const& to) override;
	void DrawEllipse(Point const& center, double width, double height) override;
	void FilEllipse(Point const& center, double width, double height) override;
	void FillPoligon(std::vector<Point> const& pointsArr) override;

private:
	GLFWwindow* m_window;
	RGBAColor m_fillColor;
	RGBAColor m_outlineColor;
	float m_lineThickness = 1;
};

