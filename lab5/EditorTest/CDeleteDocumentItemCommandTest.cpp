#include "../../catch2/catch.hpp"
#include "../Editor/CDeleteDocumentItemCommand.h"
#include "../Editor/CHistory.h"
#include "../Editor/CParagraph.h"
#include "../Editor/CImage.h"

namespace
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);
	CDocumentItem firstParagraph(std::make_shared<CParagraph>(commandFunc, "first paragraph text"));
	CDocumentItem secondParagraph(std::make_shared<CParagraph>(commandFunc, "second paragraph text"));
}

TEST_CASE("executes a command to remove an element from a specified position")
{
	std::vector<CDocumentItem> items{firstParagraph, secondParagraph};
	CDeleteDocumentItemCommand command1(items, 1);
	command1.Execute();

	CHECK(1 == items.size());
	CHECK(firstParagraph.GetParagraph() == items[0].GetParagraph());
}

TEST_CASE("impossible to remove an element from a nonexistent position")
{
	std::vector<CDocumentItem> items{ firstParagraph, secondParagraph };
	CDeleteDocumentItemCommand command1(items, 2);

	CHECK_THROWS(command1.Execute());
	CHECK(2 == items.size());
}

TEST_CASE("unexecute() returns the item back to its place")
{
	std::vector<CDocumentItem> items{ firstParagraph, secondParagraph };
	CDeleteDocumentItemCommand command1(items, 0);

	command1.Execute();

	CHECK(1 == items.size());

	command1.Unexecute();
	CHECK(2 == items.size());
	CHECK(firstParagraph.GetParagraph() == items[0].GetParagraph());
}

TEST_CASE("If the command to delete the image is executed and destroyed, then the image is deleted")
{
	Path path;
	{
		CDocumentItem image(std::make_shared<CImage>(commandFunc, "image/img.png", 600, 800));
		std::vector<CDocumentItem> items{ firstParagraph, image };
		CDeleteDocumentItemCommand command(items, 1);
		command.Execute();
		path = image.GetImage()->GetPath();
		CHECK(std::filesystem::exists(path));
	}

	CHECK_FALSE(std::filesystem::exists(path));
}