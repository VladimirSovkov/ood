#include "CDesigner.h"
#include "CShapeFactory.h"
#include "CPainter.h"
#include "CCanvas.h"
#include <iostream>

//rectangle pink 0 1 1 0

int main()
{
	try
	{
		CShapeFactory factory;
		CDesigner designer(factory);
		CPainter painter;

		CPictureDraft draft = designer.CreateDraft(std::cin);
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
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(255, 255, 255, 0);
			painter.DrawPicture(draft, canvas);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return 0;
}