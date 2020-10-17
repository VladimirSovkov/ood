#pragma once
#include "ICanvas.h"
#include <ostream>
#include <glfw3.h>

class CCanvas : public ICanvas
{
public:
	CCanvas(GLFWwindow* window);

	struct RGB
	{
		RGB(float r, float g, float b)
			:red(r)
			, green(g)
			, blue(b)
		{
		}

		float red = 0;
		float green = 0;
		float blue = 0;
	};

	void SetColor(Color color) override;
	void DrawLine(const Point& from, const Point& to) override;
	void DrawEllipse(const Point& leftTop, double width, double height) override;

private:
	GLFWwindow* m_window;
	RGB m_color;
};