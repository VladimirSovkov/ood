#include "../../catch2/catch.hpp"
#include "../Editor/CEditor.h"

namespace
{
	const std::string INSTRUCTION_TEXT = R"(Commands list:
 InsertParagraph: InsertParagraph <position>|end <paragraph text>
 InsertImage: InsertImage <position>|end <width> <height> <image file path>
 SetTitle: SetTitle <document title>
 List: list
 ReplaceText: ReplaceText <position> <paragraph text>
 ResizeImage: ResizeImage <position> <width> <height>
 DeleteItem: DeleteItem <position>
 Help: Help
 Undo: Undo
 Redo: Redo
 Save: Save <path>
 Exit: Exit
>)";
}

void CompareEditorResponseWithExpectedResult(std::string const& command, std::string const& expectedResult)
{
	std::istringstream iss(command);
	std::ostringstream oss;
	CEditor editor(iss, oss);
	editor.Run();

	CHECK(INSTRUCTION_TEXT + expectedResult == oss.str());
}

//std::istringstream iss("InsertParagraph end text test");
//std::ostringstream oss;
//CEditor editor(iss, oss);
//editor.Run();
//std::string ss = oss.str();
//int stop = 0;

TEST_CASE("InsertParagraph inserts a paragraph with text at the document position")
{
	SECTION("Insert paragraph at the end")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end", ">");
	}

	SECTION("Insert a paragraph at a specific position")
	{
		std::stringstream command;
		command << "InsertParagraph end text\n"
			<< "InsertParagraph 0 new Text\n"
			<< "InsertParagraph 1 new Text 2";
		CompareEditorResponseWithExpectedResult(command.str(), ">>>");
	}
	SECTION("invalid insert position")
	{
		std::stringstream command; 
		command << "InsertParagraph end text\n"
			<< "InsertParagraph 0 end newText\n"
			<< "InsertParagraph 2 end newText2";
		CompareEditorResponseWithExpectedResult(command.str(), ">>Document item position is out of range\n>");
	}

	SECTION("invalid command")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph pos failed test", "Invalid position argument: pos\n>");
	}
}

TEST_CASE("InsertImage() should insert an image at a specific position")
{
	SECTION("Insert image at the end")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 800 600 image/img.png", ">");
	}

	SECTION("Insert a image at a specific position")
	{
		std::stringstream command;
		command << "InsertImage end 800 600 image/img.png\n"
			<< "InsertImage 0 1200 1000 image/img.png\n"
			<< "InsertImage 1 1920 1080 image/img.png";
		CompareEditorResponseWithExpectedResult(command.str(), ">>>");
	}

	SECTION("incorrect path specified")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 800 600 noDirectory/img.png", "invalid image path\n>");
	}

	SECTION("incorrect image format")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 800 600 image/img.txt", "invalid image path\n>");
	}

	SECTION("width and height are incorrect")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 0 1 image/img.png", "invalid expression sizes\n>");
	}

	SECTION("invalid insert position")
	{
		std::stringstream command;
		command << "InsertImage end 800 600 image/img.png\n"
			<< "InsertImage 0 1200 1000 image/img.png\n"
			<< "InsertImage 2 1920 1080 image/img.png";
		CompareEditorResponseWithExpectedResult(command.str(), ">>Document item position is out of range\n>");
	}	
	
	SECTION("invalid command")
	{
		CompareEditorResponseWithExpectedResult("InsertImage pos 800 600 image/img.png", "Invalid position argument: pos\n>");
	}
}

TEST_CASE("SetTitle: sets the title of the document")
{
	CompareEditorResponseWithExpectedResult("SetTitle title", ">");
	CompareEditorResponseWithExpectedResult("SetTitle", ">");
}

TEST_CASE("ReplaceText: replaces the text in the paragraph specified in position")
{
	SECTION("correct text replacement")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nReplaceText 0 new text", ">>");
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nReplaceText 0 new text", ">>");
	}

	SECTION("replacing text at image position")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 800 600 image/img.png\nReplaceText 0 new text", ">The document element at position 0 is not a paragraph\n>");
	}

	SECTION("incorrect position")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nReplaceText 2 new text", ">invalid vector subscript\n>");
	}
}

TEST_CASE("ResizeImage: Resizes the image in the document")
{
	SECTION("correct image resizing")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 800 600 image/img.png\nResizeImage 0 1920 1080", ">>");
	}

	SECTION("a paragraph is in the specified position")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nResizeImage 0 1920 1080", ">The document element at position 0 is not a image\n>");
	}

	SECTION("incorrect position")
	{
		CompareEditorResponseWithExpectedResult("InsertImage end 800 600 image/img.png\nResizeImage 2 1920 1080", ">invalid vector subscript\n>");
	}

	//SECTION("incorrect dimensions")
	//{
	//	CompareEditorResponseWithExpectedResult("InsertImage end 800 600 image/img.png\nResizeImage 0 0 0", ">>");
	//}
}

TEST_CASE("DeleteItem: deletes the item at the specified position")
{
	SECTION("correct deletion")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nDeleteItem 0", ">>");
	}

	SECTION("no element in position")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nDeleteItem 2", ">Document item position is out of range\n>");
	}

	SECTION("incorrect position")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nDeleteItem pos", ">Invalid position argument: pos\n>");
	}
}

TEST_CASE("Help: displays available commands")
{
	CompareEditorResponseWithExpectedResult("Help", INSTRUCTION_TEXT );
}

TEST_CASE("Undo: cancels the action of a previously entered command")
{
	SECTION("correct command cancellation")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nDeleteItem 0\nUndo\nReplaceText 0 new text", ">>>>");
	}
	SECTION("canceling a command that is not")
	{
		CompareEditorResponseWithExpectedResult("Undo", "Can't undo\n>");
	}
}

TEST_CASE("Redo: redo the canceled command")
{
	SECTION("correct redo of a canceled command")
	{
		CompareEditorResponseWithExpectedResult("InsertParagraph end text\nDeleteItem 0\nUndo\nRedo\nReplaceText 0 new text", ">>>>invalid vector subscript\n>");
	}

	SECTION("redo without canceled commands")
	{
		CompareEditorResponseWithExpectedResult("Redo", "Can't redo\n>");
	}
}

TEST_CASE("Save: saves html document")
{
	CompareEditorResponseWithExpectedResult("Save DeleteMe.html", ">");
	SECTION("passed an empty document path")
	{
		CompareEditorResponseWithExpectedResult("Save", "Empty save directory given\n>");
	}
}

TEST_CASE("List: print all elements of document")
{
	std::stringstream command;
	std::stringstream expectedResult;
	command << "SetTitle HtmlDocument\n"
		<< "InsertParagraph end text\n"
		<< "InsertParagraph end new text\n"
		<< "List";

	expectedResult << "Title: HtmlDocument\n"
		<< "0. Paragraph: text\n"
		<< "1. Paragraph: new text";

	CompareEditorResponseWithExpectedResult(command.str(), ">>>" + expectedResult.str() + "\n>");
}