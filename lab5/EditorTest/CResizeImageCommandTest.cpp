#include "../../catch2/catch.hpp"
#include "../Editor/CResizeImageCommand.h"

TEST_CASE("resize images with Resize Image Command")
{
	int width = 800;
	int height = 600;
	int newWidth = 1920;
	int newHeight = 1080;
	CResizeImageCommand command(width, height, newWidth, newHeight);
	
	SECTION("execute command")
	{
		command.Execute();
		CHECK(1920 == width);
		CHECK(1080 == height);
	}

	SECTION("unexecute command")
	{
		command.Unexecute();
		CHECK(800 == width);
		CHECK(600 == height);
	}
}

TEST_CASE("incorrect dimensions for resizing")
{
	int width = 800;
	int height = 600;
	int newWidth = 0;
	int newHeight = 1080;
	CHECK_THROWS(CResizeImageCommand (width, height, newWidth, newHeight));
}