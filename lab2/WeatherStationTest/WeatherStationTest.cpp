#define CATCH_CONFIG_MAIN
#include "../../catch2/catch.hpp"
#include "ObserversTest.h"

TEST_CASE("removing yourself from the observer list in the Update method")
{
	CWeatherData wd;
	CRemoveDisplay removeDisplay(wd);
	wd.RegisterObserver(removeDisplay, 1);
	wd.SetMeasurements(5, 0.9, 764);
	wd.SetMeasurements(4, 0.8, 763);
	wd.RemoveObserver(removeDisplay);
}

TEST_CASE("priority notification to observers")
{
	CWeatherData wd;
	std::stringstream stringStream;
	ObserverWithStringStream observerFirst(stringStream, "Hel");
	ObserverWithStringStream observerSecond(stringStream, "lo ");
	ObserverWithStringStream observerThird(stringStream, "wor");
	ObserverWithStringStream observerFourth(stringStream, "ld!");

	wd.RegisterObserver(observerThird, 2);
	wd.RegisterObserver(observerSecond, 3);
	wd.RegisterObserver(observerFirst, 4);
	wd.RegisterObserver(observerFourth, 1);

	wd.SetMeasurements(5, 0.9, 764);
	CHECK(stringStream.str() == "Hello world!");
}