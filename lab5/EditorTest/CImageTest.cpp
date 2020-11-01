#include "../../catch2/catch.hpp"
#include "../Editor/CImage.h"
#include "../Editor/CHistory.h"

namespace 
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);
}

TEST_CASE("object creation by constructor")
{
	CImage image(commandFunc, "image/img.png", 800, 600);

	CHECK(800 == image.GetWidth());
	CHECK(600 == image.GetHeight());
	CHECK(std::filesystem::exists(image.GetPath()));
}

TEST_CASE("if destroyed, the image file should be deleted")
{
	CImage image(commandFunc, "image/img.png", 800, 600);
	Path path = image.GetPath();
	
	CHECK(std::filesystem::exists(path));

	image.~CImage();
	CHECK_FALSE(std::filesystem::exists(path));
}

TEST_CASE("Resize() should resize the image")
{
	CImage image(commandFunc, "image/img.png", 800, 600);
	int width = 1920;
	int height = 1080;
	image.Resize(1920, 1080);
	
	CHECK(width == image.GetWidth());
	CHECK(height == image.GetHeight());
}