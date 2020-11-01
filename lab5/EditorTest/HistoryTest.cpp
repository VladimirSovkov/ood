#include "../../catch2/catch.hpp"
#include "../Editor/CHistory.h"
#include "../Editor/CReplaceTextCommand.h"
#include "../Editor/CResizeImageCommand.h"

TEST_CASE("CanUndo() checks for can undo commands")
{
	CHistory history;

	SECTION("command history is empty")
	{
		CHECK_FALSE(history.CanUndo());
	}

	SECTION("there are commands in history")
	{
		int width = 100;
		int height = 200;
		history.AddAndExecuteCommand(std::make_unique<CResizeImageCommand>(width, height, 800, 600));
		history.AddAndExecuteCommand(std::make_unique<CResizeImageCommand>(width, height, 100, 200));
		CHECK(history.CanUndo());
		
		history.Undo();
		CHECK(history.CanRedo());
	}
}

TEST_CASE("CanRedo() checks for can redo commands")
{
	CHistory history;

	SECTION("story is empty")
	{
		CHECK_FALSE(history.CanRedo());
	}

	SECTION("there are records in history")
	{
		std::string str1 = "text";
		std::string str2 = "nextText";
		history.AddAndExecuteCommand(std::make_unique<CReplaceTextCommand>(str1, str2));
		history.AddAndExecuteCommand(std::make_unique<CReplaceTextCommand>(str1, str2));

		CHECK_FALSE(history.CanRedo());

		history.Undo();
		CHECK(history.CanRedo());
	}
}

TEST_CASE("check  undo and redo commands work")
{
	CHistory history;
	int width = 100;
	int height = 200;
	history.AddAndExecuteCommand(std::make_unique<CResizeImageCommand>(width, height, 800, 600));
	history.AddAndExecuteCommand(std::make_unique<CResizeImageCommand>(width, height, 100, 200));
	
	CHECK(history.CanUndo());
	history.Undo();

	CHECK(history.CanUndo());
	history.Undo();

	CHECK_FALSE(history.CanUndo());

	CHECK(history.CanRedo());
	history.Redo();
	
	CHECK(history.CanRedo());
	history.Redo();
	
	CHECK_FALSE(history.CanRedo());
}

TEST_CASE("AddAndExecuteCommand () the size of the command history is limited to 10 commands")
{
	CHistory history;

	std::string str1 = "text 1";
	std::string str2 = "text 2";

	for (size_t i = 0; i < 13; i++)
	{
		history.AddAndExecuteCommand(std::make_unique<CReplaceTextCommand>(str1, str2));
	}

	for (size_t i = 0; i < 10; i++)
	{
		CHECK(history.CanUndo());
		history.Undo();
	}

	CHECK_FALSE(history.CanUndo());
}