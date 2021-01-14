#include "../MultiGumballMachineWithRefill/GumBallMachineWithState.h"
#include "../../catch2/catch.hpp"
#include "RedirectCout.h"

using namespace with_state;

namespace
{
	std::string CreateStateGumballMachine(unsigned numBalls, unsigned quarterCount, std::string nameState)
	{
		std::ostringstream ss;
		ss << "Mighty Gumball, Inc.\n"
			<< "C++-enabled Standing Gumball Model #2016 (with state)\n"
			<< "Inventory: " << numBalls << " gumball" << (numBalls != 1 ? "s, " : ", ")
			<< quarterCount << " quarter" << (quarterCount != 1 ? "s\n" : "\n")
			<< "Machine is " << nameState << std::endl;
		return ss.str();
	}
}

TEST_CASE("Refill GumballMachine with gumballs in sold out state")
{
	CGumballMachine machine(0);
	CHECK(CreateStateGumballMachine(0, 0, "sold out") == machine.ToString());

	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	SECTION("Add gumballs in machine and state will change to NoQuarterState")
	{
		machine.Refill(1);
		
		CHECK("Filled with 1 balls\n" == ss.str());
		CHECK(CreateStateGumballMachine(1, 0, "waiting for quarter") == machine.ToString());
	}

	SECTION("Adding chewing gum to a machine that has quarters")
	{
		CGumballMachine machine(1);
		machine.InsertQuarter();
		machine.InsertQuarter();
		machine.InsertQuarter();
		machine.TurnCrank();
		CHECK(CreateStateGumballMachine(0, 2, "sold out") == machine.ToString());

		ss.str("");
		machine.Refill(1);

		CHECK("Filled with 1 balls\n" == ss.str());
		CHECK(CreateStateGumballMachine(1, 2, "waiting for turn of crank or insert of quarter") == machine.ToString());
	}
}

TEST_CASE("Refill machine with chewing balls in CNoQuarterState, without changing the state")
{
	CGumballMachine machine(5);
	CHECK(CreateStateGumballMachine(5, 0, "waiting for quarter") == machine.ToString());

	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	machine.Refill(1);
	
	CHECK("Filled with 1 balls\n" == ss.str());
	CHECK(CreateStateGumballMachine(6, 0, "waiting for quarter") == machine.ToString());
}

TEST_CASE("Refill machine with chewing balls in HasQuarterState without changing state")
{
	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	CGumballMachine machine(5);
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	CHECK(CreateStateGumballMachine(5, 3, "waiting for turn of crank or insert of quarter") == machine.ToString());

	ss.str("");
	machine.Refill(1);
	
	CHECK("Filled with 1 balls\n" == ss.str());
	CHECK(CreateStateGumballMachine(6, 3, "waiting for turn of crank or insert of quarter") == machine.ToString());
	
}

TEST_CASE("Refill gumballs in  max quarter count state, don't change it")
{
	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	CGumballMachine machine(5);
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	CHECK(CreateStateGumballMachine(5, 5, "waiting for turn of crank with maximum quarters") == machine.ToString());

	ss.str("");
	machine.Refill(1);

	CHECK("Filled with 1 balls\n" == ss.str());
	CHECK(CreateStateGumballMachine(6, 5, "waiting for turn of crank with maximum quarters") == machine.ToString());
}