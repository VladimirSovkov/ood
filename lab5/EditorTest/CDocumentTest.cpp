#include "../../catch2/catch.hpp"
#include "../Editor/CDocument.h"
#include <fstream>
#include <sstream>

TEST_CASE("insert text into a paragraph")
{
	CDocument document;
	document.InsertParagraph("Hello world!");
	
	CHECK(1u == document.GetItemsCount());
	CHECK("Hello world!" == document.GetItem(0).GetParagraph()->GetText());
	

	SECTION("add to the end")
	{
		document.InsertParagraph("text");
		CHECK(2u == document.GetItemsCount());
		CHECK("text" == document.GetItem(1).GetParagraph()->GetText());
	}

	SECTION("add to the beginning")
	{
		document.InsertParagraph("textFirst", 0);
		CHECK(2u == document.GetItemsCount());
		CHECK("textFirst" == document.GetItem(0).GetParagraph()->GetText());
	}

	document.InsertParagraph("end text");

	SECTION("add in the middle")
	{
		document.InsertParagraph("text in the middle", 1);
		CHECK(3u == document.GetItemsCount());
		CHECK("text in the middle" == document.GetItem(1).GetParagraph()->GetText());
	}
}

TEST_CASE("insert an image into a paragraph")
{
	CDocument document;
	document.InsertImage("image/img.png", 800, 600);
	CHECK(1u == document.GetItemsCount());
	CHECK(800 == document.GetItem(0).GetImage()->GetWidth());
	CHECK(600 == document.GetItem(0).GetImage()->GetHeight());
	
	SECTION("add to the end")
	{
		document.InsertImage("image/img.png", 1920, 1080);
		CHECK(2u == document.GetItemsCount());
		CHECK(1920 == document.GetItem(1).GetImage()->GetWidth());
		CHECK(1080 == document.GetItem(1).GetImage()->GetHeight());
	}

	SECTION("add to the beginning")
	{
		document.InsertImage("image/img.png", 1400, 1050, 0);
		CHECK(2u == document.GetItemsCount());
		CHECK(1400 == document.GetItem(0).GetImage()->GetWidth());
		CHECK(1050 == document.GetItem(0).GetImage()->GetHeight());
	}

	document.InsertImage("image/img.png", 1200, 900);

	SECTION("add in the middle")
	{
		document.InsertImage("image/img.png", 1080, 1350, 1);
		CHECK(3u == document.GetItemsCount());
		CHECK(1080 == document.GetItem(1).GetImage()->GetWidth());
		CHECK(1350 == document.GetItem(1).GetImage()->GetHeight());
	}
}

TEST_CASE("insert an image and a paragraph into the document")
{
	CDocument document;
	document.InsertParagraph("text with");
	document.InsertImage("image/img.png", 800, 600);
	document.InsertParagraph(".");

	CHECK(3u == document.GetItemsCount());
	CHECK("text with" == document.GetItem(0).GetParagraph()->GetText());

	CHECK(800 == document.GetItem(1).GetImage()->GetWidth());
	CHECK("." == document.GetItem(2).GetParagraph()->GetText());

	SECTION("get an invalid item")
	{
		CHECK(nullptr == document.GetItem(0).GetImage());
		CHECK(nullptr == document.GetItem(1).GetParagraph());
	}
}

TEST_CASE("remove item from position")
{
	SECTION("correct deletion")
	{
		CDocument document;
		document.InsertParagraph("text");
		document.InsertImage("image/img.png", 800, 600);
		document.InsertParagraph(".");

		document.DeleteItem(0);
		CHECK(2u == document.GetItemsCount());
		CHECK(800 == document.GetItem(0).GetImage()->GetWidth());

		document.DeleteItem(1);
		CHECK(1u == document.GetItemsCount());
		CHECK(800 == document.GetItem(0).GetImage()->GetWidth());

		document.DeleteItem(0);
		CHECK(0u == document.GetItemsCount());
	}

	SECTION("remove out of range item")
	{
		CDocument document;
		document.InsertParagraph("text");

		CHECK_THROWS(document.DeleteItem(2));

		document.DeleteItem(0);
		CHECK_THROWS(document.DeleteItem(0));
	}
}

TEST_CASE("SetTitle should set title")
{
	CDocument document;
	document.SetTitle("title");
	
	CHECK("title" == document.GetTitle());

	document.SetTitle("new title");
	CHECK("new title" == document.GetTitle());
}

TEST_CASE("CanUndo should return a cancellation response")
{
	CDocument document;

	CHECK_FALSE(document.CanUndo());
	
	document.InsertParagraph("text");
	CHECK(document.CanUndo());

	document.Undo();
	CHECK_FALSE(document.CanUndo());
}

TEST_CASE("Undo should undo the previous command")
{
	CDocument document;
	document.InsertImage("image/img.png", 800, 600);
	document.InsertParagraph("text");

	CHECK_NOTHROW(document.Undo());
	CHECK(1u == document.GetItemsCount());
	CHECK(800 == document.GetItem(0).GetImage()->GetWidth());

	CHECK_NOTHROW(document.Undo());
	CHECK(0u == document.GetItemsCount());

	document.Undo();
	CHECK(0u == document.GetItemsCount());
}

TEST_CASE("CanRedo should return redo permission")
{
	CDocument document;
	document.InsertImage("image/img.png", 800, 600);
	document.InsertParagraph("text");
	document.Undo();
	
	CHECK(document.CanRedo());

	document.Redo();
	CHECK_FALSE(document.CanRedo());
}

TEST_CASE("Redo must redo the canceled command")
{
	CDocument document;
	document.InsertParagraph("text");
	document.InsertImage("image/img.png", 800, 600);
	document.Undo();
	CHECK(1u == document.GetItemsCount());
	CHECK("text" == document.GetItem(0).GetParagraph()->GetText());

	document.Redo();
	CHECK(2u == document.GetItemsCount());
	CHECK(800 == document.GetItem(1).GetImage()->GetWidth());

	document.Redo();
	CHECK(2u == document.GetItemsCount());
}

std::string FileToStr(std::string const& fileName)
{
	std::ifstream file(fileName);
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

TEST_CASE("saving document")
{
	CDocument document;
	document.SetTitle("test title");
	document.InsertParagraph("text");
	document.InsertImage("image/img.png", 800, 600);
	std::string directory = "output";
	if (!std::filesystem::is_directory(directory))
	{
		std::filesystem::create_directory(directory);
	}
	std::string filePath = directory + "/index.html";

	document.Save(filePath);
	std::string fileData = FileToStr(filePath);
	std::stringstream sampleData;

	sampleData << "<!DOCTYPE html>\n"
		<< "<html>\n"
		<< "<head>\n"
		<< "\t<title>test title</title>\n"
		<< "</head>\n"
		<< "<body>\n"
		<< "<p>text</p>\n"
		<< "<img src=" << document.GetItem(1).GetImage()->GetPath() << " width=\"800\" height=\"600\">\n"
		<< "</body>\n"
		<< "</html>\n";
	CHECK(sampleData.str() == fileData);
}