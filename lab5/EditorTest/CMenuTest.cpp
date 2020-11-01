#include "../../catch2/catch.hpp"
#include "../Editor/CMenu.h"
#include <sstream>

TEST_CASE("AddItem adds commands to the menu")
{
	std::istringstream istream("test");
	std::ostringstream ostream;
	CMenu menu(istream, ostream);

	std::stringstream ss;
	menu.AddItem("test", "test description", [&](std::istream& in) { ss << "command executed"; });
	menu.Run();
	
	CHECK("command executed" == ss.str());
	CHECK("Commands list:\n test: test description\n>>" == ostream.str());
}

TEST_CASE("ShowInstruction should output all commands")
{
	std::istringstream istream;
	std::ostringstream ostream;
	CMenu menu(istream, ostream);

	menu.AddItem("command first", "description first", [](std::istream& in) {});
	menu.AddItem("command second", "description second", [](std::istream& in) {});

	menu.ShowInstructions();
	std::string sampleInstruct = "Commands list:\n command first: description first\n command second: description second\n";
	CHECK(sampleInstruct == ostream.str());
}

TEST_CASE("displaying a message about an invalid command")
{
	std::istringstream istream("invalidCommand");
	std::ostringstream ostream;
	CMenu menu(istream, ostream);

	menu.AddItem("command", "description", [](std::istream& in) {});
	menu.Run();

	std::string sample = "Commands list:\n command: description\n>Unknown command\n>";
	CHECK(sample == ostream.str());
}

TEST_CASE("Stop() stops execution of commands")
{
	std::istringstream istream("test\nexit\ntest");
	std::ostringstream ostream;
	CMenu menu(istream, ostream);

	std::stringstream ss;
	menu.AddItem("test", "d", [&](std::istream&) {ss << "test executed"; });
	menu.AddItem("exit", "d", [&](std::istream&) {menu.Exit(); });
	menu.Run();
	CHECK("test executed" == ss.str());
}