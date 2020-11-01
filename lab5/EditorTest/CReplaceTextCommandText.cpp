#include "../../catch2/catch.hpp"
#include "../Editor/CReplaceTextCommand.h"

TEST_CASE("Swap strings in CReplaceTextCommand")
{
	std::string replaceableText = "Hello ";
	std::string replacementText = "world!";

	CReplaceTextCommand replaceText(replaceableText, replacementText);

	replaceText.Execute();
	CHECK("world!" == replaceableText);

	replaceText.Unexecute();
	CHECK("Hello " == replaceableText);
}