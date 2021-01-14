#include "../../catch2/catch.hpp"
#include "../GumballMachine/GumBallMachine.h"
#include <sstream>

using namespace with_state;

struct RedirectCout
{
	RedirectCout(std::streambuf* new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{
	}

	~RedirectCout()
	{
		std::cout.rdbuf(old);
	}

private:
	std::streambuf* old;
};

std::string CreateStateGumballMachine(unsigned numBalls, std::string nameState)
{
	std::ostringstream ss;
	ss << "Mighty Gumball, Inc.\n"
		<< "C++-enabled Standing Gumball Model #2016 (with state)\n"
		<< "Inventory: " << numBalls << " gumball" << (numBalls != 1 ? "s\n" : "\n")
		<< "Machine is " << nameState << std::endl;
	return ss.str();
}

TEST_CASE("Gumball Machine has gumball")
{
	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	CGumballMachine machine(5);
	CHECK(CreateStateGumballMachine(5, "waiting for quarter") == machine.ToString());

	SECTION("InsertQuarter() will change state of machine to HasQuarterState")
	{
		ss.str("");
		machine.InsertQuarter();
		CHECK("You inserted a quarter\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, "waiting for turn of crank") == machine.ToString());
	}

	SECTION("TurnCrank() machine gives chewing gum and changes state to NoQuarterState")
	{
		machine.InsertQuarter();
		ss.str("");
		machine.TurnCrank();
		std::ostringstream sample;
		sample << "You turned...\n"
			<< "A gumball comes rolling out the slot...\n";
		CHECK(sample.str() == ss.str());
		CHECK(CreateStateGumballMachine(4, "waiting for quarter") == machine.ToString());
	}

	SECTION("TurnCrank() machine gives chewing gum and changes state to SoldOutState")
	{
		CGumballMachine machine(1);
		machine.InsertQuarter();
		ss.str("");
		machine.TurnCrank();
		
		std::ostringstream sample;
		sample << "You turned...\n"
			<< "A gumball comes rolling out the slot...\n"
			<< "Oops, out of gumballs\n";

		CHECK(sample.str() == ss.str());
		CHECK(CreateStateGumballMachine(0, "sold out") == machine.ToString());
	}

	SECTION("EjectQuarter() returns a quarter and changes state to NoQuarterState")
	{
		machine.InsertQuarter();
		ss.str("");
		machine.EjectQuarter();

		CHECK("Quarter returned\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, "waiting for quarter") == machine.ToString());
	}
}

TEST_CASE("Gumball Machine has no gum balls")
{
	CGumballMachine machine(0);
	CHECK(CreateStateGumballMachine(0, "sold out") == machine.ToString());
	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());
	
	SECTION("InsertQuarter() will not change state of machine")
	{
		ss.str("");
		machine.InsertQuarter();
		CHECK("You can't insert a quarter, the machine is sold out\n" == ss.str());
		CHECK(CreateStateGumballMachine(0, "sold out") == machine.ToString());
	}

	SECTION("TurnCrank() will not change state of machine")
	{
		ss.str("");
		machine.TurnCrank();
		CHECK("You turned but there's no gumballs\nNo gumball dispensed\n" == ss.str());
		CHECK(CreateStateGumballMachine(0, "sold out") == machine.ToString());
	}

	SECTION("EjectQuarter() will not change state of machine") 
	{
		ss.str("");
		machine.EjectQuarter();
		CHECK("You can't eject, you haven't inserted a quarter yet\n" == ss.str());
		CHECK(CreateStateGumballMachine(0, "sold out") == machine.ToString());
	}
}

TEST_CASE("GumBall machine is always in state no quarter state")
{
	CGumballMachine machine(4);
	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	SECTION("EjectQuarter() will not change state of machine")
	{
		ss.str("");
		machine.EjectQuarter();

		CHECK("You haven't inserted a quarter\n" == ss.str());
		CHECK(CreateStateGumballMachine(4, "waiting for quarter") == machine.ToString());
	}

	SECTION("TurnCrank() will not change state of machine")
	{
		ss.str("");
		machine.TurnCrank();
		
		std::ostringstream sample;
		sample << "You turned but there's no quarter\n"
			<< "You need to pay first\n";

		CHECK(sample.str() == ss.str());
		CHECK(CreateStateGumballMachine(4, "waiting for quarter") == machine.ToString());
	}
}