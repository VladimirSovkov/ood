#include "../../catch2/catch.hpp"
#include "../Editor/CInsertDocumentItemCommand.h"
#include "../Editor/CParagraph.h"
#include "../Editor/CImage.h"
#include "../Editor/CHistory.h"

TEST_CASE("Adding an item at a specified position")
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);

	std::vector<CDocumentItem> items;
	CDocumentItem documentItem1(std::make_shared<CParagraph>(commandFunc, "text"));
	CInsertDocumentItemCommand command1(documentItem1, items);
	command1.Execute();
	CHECK(items.back().GetParagraph() == documentItem1.GetParagraph());

	CDocumentItem documentItem2(std::make_shared<CParagraph>(commandFunc, "text2"));
	CInsertDocumentItemCommand command2(documentItem2, items, 0);
	command2.Execute();
	CHECK(items.front().GetParagraph() == documentItem2.GetParagraph());

	CDocumentItem documentItem3(std::make_shared<CParagraph>(commandFunc, "text3"));
	CInsertDocumentItemCommand command3(documentItem3, items);
	command3.Execute();
	CHECK(items.back().GetParagraph() == documentItem3.GetParagraph());

	CHECK(items[1].GetParagraph() == documentItem1.GetParagraph());
}

TEST_CASE("cancel command execution")
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);

	std::vector<CDocumentItem>items;
	CDocumentItem documentItem1(std::make_shared<CParagraph>(commandFunc, "text1"));
	CInsertDocumentItemCommand command1(documentItem1, items);
	command1.Execute();

	CDocumentItem documentItem2(std::make_shared<CParagraph>(commandFunc, "text1"));
	CInsertDocumentItemCommand command2(documentItem2, items);
	command2.Execute();

	command2.Unexecute();
	CHECK(items.back().GetParagraph() == documentItem1.GetParagraph());

	command1.Unexecute();
	CHECK(items.size() == 0);
}

TEST_CASE("Insert an element in an incorrect position")
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);
	std::vector<CDocumentItem>items;
	CDocumentItem documentItem1(std::make_shared<CParagraph>(commandFunc, "text1"));
	CInsertDocumentItemCommand command1(documentItem1, items, 1);
	CHECK_THROWS(command1.Execute());
}

TEST_CASE("Undo a canceled command")
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);
	std::vector<CDocumentItem>items;
	CDocumentItem documentItem1(std::make_shared<CParagraph>(commandFunc, "text1"));
	CInsertDocumentItemCommand command1(documentItem1, items);
	command1.Execute();
	
	command1.Unexecute();
	CHECK(0 == items.size());
	command1.Unexecute();
	CHECK(0 == items.size());
}

TEST_CASE("destruction of the image when canceling and deleting the command to insert an image")
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);
	Path path;
	std::vector<CDocumentItem>items;

	{
		CDocumentItem image(std::make_shared<CImage>(commandFunc, "image/img.png", 600, 800));
		path = image.GetImage()->GetPath();
		CInsertDocumentItemCommand command(image, items);
		command.Execute();
		CHECK(image.GetImage() == items.back().GetImage());

		command.Unexecute();
		CHECK(std::filesystem::exists(path));
	}
	
	CHECK_FALSE(std::filesystem::exists(path));	
}