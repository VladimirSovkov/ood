#include <glfw3.h>
#include "CEllipse.h"
#include "CRectangle.h"
#include "CTriangle.h"
#include"CShapeGroup.h"
#include "CCanvas.h"

int main()
{
	auto rectangle = std::make_shared<CRectangle>(Point{ 0.1, 0.3 }, 0.3, 0.3);
	rectangle->GetFillStyle()->SetColor(RGBAColor(1.f, 0.65f, 0, 0 ));
	rectangle->GetOutlineStyle()->SetColor(RGBAColor(0, 0.69f, 0.39f, 0));
	rectangle->GetOutlineStyle()->SetLineThickness(5);

	auto triangle = std::make_shared<CTriangle>(Point{ 0, 0.3 }, Point{0.25, 0.5}, Point{0.5, 0.3});
	triangle->GetFillStyle()->SetColor(RGBAColor(0, 0.69f, 0.39f, 0));
	triangle->GetOutlineStyle()->SetColor(RGBAColor(0, 0, 0, 0));
	triangle->GetOutlineStyle()->SetLineThickness(1.5);

	auto ellipse = std::make_shared<CEllipse>(Point{ 0.25, 0.15 }, 0.09, 0.05);
	ellipse->GetFillStyle()->SetColor(RGBAColor(0.44f, 0.99f, 1.f, 0));
	ellipse->GetOutlineStyle()->Enable(false);

	CShapeGroup shapeGroup;
	shapeGroup.AddShape(rectangle);
	shapeGroup.AddShape(triangle);
	shapeGroup.AddShape(ellipse);

	GLFWwindow* window{};
	CCanvas canvas(window);

	if (!glfwInit())
		return 1;
	window = glfwCreateWindow(1024, 768, "Canvas", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glClearColor(255, 255, 255, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		shapeGroup.Draw(canvas);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
