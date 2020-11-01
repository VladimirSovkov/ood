#include "../../catch2/catch.hpp"
#include "../Editor/CParagraph.h"
#include "../Editor/CHistory.h"

namespace
{
	CHistory history;
	CommandFunc commandFunc = std::bind(&CHistory::AddAndExecuteCommand, &history, std::placeholders::_1);
}

TEST_CASE("creating a paragraph by the constructor")
{

	CParagraph paragraph(commandFunc, "text");
	
	CHECK("text" == paragraph.GetText());
	CHECK_FALSE(history.CanUndo());
}

TEST_CASE("SetText() sets new text in the paragraph")
{
	CParagraph paragraph(commandFunc, "text");

	paragraph.SetText("new text");
	CHECK("new text" == paragraph.GetText());
	CHECK(history.CanUndo());
}