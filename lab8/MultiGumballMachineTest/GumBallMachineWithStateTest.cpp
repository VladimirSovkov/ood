#include "../../catch2/catch.hpp"
#include "../MultiGumballMachine/GumBallMachineWithState.h"
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

TEST_CASE("GumballMachine is in has quarter state")
{
	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	CGumballMachine machine(5);
	machine.InsertQuarter();
	CHECK(CreateStateGumballMachine(5, 1, "waiting for turn of crank or insert of quarter") == machine.ToString());

	SECTION("InsertQuarter() will add a quarter, and not change the state of the machine")
	{
		ss.str("");
		machine.InsertQuarter();
		
		CHECK("You inserted another quarter\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 2, "waiting for turn of crank or insert of quarter") == machine.ToString());
	}

	SECTION("EjectQuarter() returns all coins and changes state to NoQuarterState")
	{
		machine.InsertQuarter();
		machine.InsertQuarter();
		ss.str("");
		machine.EjectQuarter();

		CHECK("3 quarters rolling out of the slot...\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 0, "waiting for quarter") == machine.ToString());
	}

	SECTION("TurnCrank() takes a coin and dispenses a gumball. Doesn't change state")
	{
		machine.InsertQuarter();
		machine.InsertQuarter();
		ss.str("");
		machine.TurnCrank();

		CHECK("You turned...\nA gumball comes rolling out the slot...\n" == ss.str());
		CHECK(CreateStateGumballMachine(4, 2, "waiting for turn of crank or insert of quarter") == machine.ToString());
		
		ss.str("");
		machine.TurnCrank();
		CHECK("You turned...\nA gumball comes rolling out the slot...\n" == ss.str());
		CHECK(CreateStateGumballMachine(3, 1, "waiting for turn of crank or insert of quarter") == machine.ToString());
	}

	SECTION("TurnCrank() picks up last coin and spits out a gumball. Changes state to NoQuarterState")
	{
		ss.str("");
		machine.TurnCrank();

		CHECK("You turned...\nA gumball comes rolling out the slot...\n" == ss.str());
		CHECK(CreateStateGumballMachine(4, 0, "waiting for quarter") == machine.ToString());
	}

	SECTION("TurnCrank() takes a coin and gives out last gumball. Changes state to SoldOutState")
	{
		CGumballMachine machine(1);
		machine.InsertQuarter();
		machine.InsertQuarter();
		machine.InsertQuarter();
		ss.str("");
		machine.TurnCrank();

		std::ostringstream sample;
		sample << "You turned...\n"
			<< "A gumball comes rolling out the slot...\n"
			<< "Oops, out of gumballs\n";

		CHECK(sample.str() == ss.str());
		CHECK(CreateStateGumballMachine(0, 2, "sold out") == machine.ToString());
	}
}

TEST_CASE("Gumball Machine is in sold out state")
{
	CGumballMachine machine(0);
	CHECK(CreateStateGumballMachine(0, 0, "sold out") == machine.ToString());

	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	SECTION("InsertQuarter() will add a quarter, not change the state of the machine")
	{
		machine.InsertQuarter();
		CHECK("You can't insert a quarter, the machine is sold out\n" == ss.str());
		CHECK(CreateStateGumballMachine(0, 0, "sold out") == machine.ToString());
	}

	SECTION("TurnCrank() not change the state of the machine")
	{
		machine.TurnCrank();
		CHECK("You turned but there's no gumballs\nNo gumball dispensed\n" == ss.str());
		CHECK(CreateStateGumballMachine(0, 0, "sold out") == machine.ToString());
	}

	SECTION("EjectQuarter() not change the state of the machine")
	{
		machine.EjectQuarter();
		CHECK("You can't eject, you haven't inserted a quarter yet\n" == ss.str());
		CHECK(CreateStateGumballMachine(0, 0, "sold out") == machine.ToString());
	}
}

TEST_CASE("Gumball Machine is in no qarter state")
{
	CGumballMachine machine(5);
	CHECK(CreateStateGumballMachine(5, 0, "waiting for quarter") == machine.ToString());

	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	SECTION("InsertQuarter() inserts a quarter into machine, changes state to HasQuarterState")
	{
		machine.InsertQuarter();
		CHECK("You inserted a quarter\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 1, "waiting for turn of crank or insert of quarter") == machine.ToString());
	}

	SECTION("TurnCrank() does not change state of machine")
	{
		machine.TurnCrank();
		CHECK("You turned but there's no quarter\nYou need to pay first\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 0, "waiting for quarter") == machine.ToString());
	}

	SECTION("EjectQuarter() does not change state of machine")
	{
		machine.EjectQuarter();
		CHECK("You haven't inserted a quarter\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 0, "waiting for quarter") == machine.ToString());
	}
}

TEST_CASE("Gumball Machine is in max quarter count state")
{
	CGumballMachine machine(5);
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	CHECK(CreateStateGumballMachine(5, 5, "waiting for turn of crank with maximum quarters") == machine.ToString());

	std::ostringstream ss;
	RedirectCout buff(ss.rdbuf());

	SECTION("InsertQuarter() does not change state of the machine")
	{
		machine.InsertQuarter();

		CHECK("You can't insert another quarter\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 5, "waiting for turn of crank with maximum quarters") == machine.ToString());
	}

	SECTION("TurnCrank() gives a gumball, changes number of quarters and state of machine to HasQuarterState")
	{
		machine.TurnCrank();

		CHECK("You turned...\nA gumball comes rolling out the slot...\n" == ss.str());
		CHECK(CreateStateGumballMachine(4, 4, "waiting for turn of crank or insert of quarter") == machine.ToString());
	}

	SECTION("EjectQuarter() will give out all quarters and change state of machine to NoQuarterState")
	{
		machine.EjectQuarter();

		CHECK("5 quarters rolling out of the slot...\n" == ss.str());
		CHECK(CreateStateGumballMachine(5, 0, "waiting for quarter") == machine.ToString());
	}
}