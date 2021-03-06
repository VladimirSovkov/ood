﻿#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "ObserversTest.h"
TEST_CASE("display of indicators from two stations at once")
{
	CWeatherData wdInside(SensorType::Internal);
	CWeatherData wdOutside(SensorType::External);
	std::stringstream stringStream;
	ObserverWithTypeSensorOutput observer(stringStream);
	wdInside.RegisterObserver(observer, 1);
	wdOutside.RegisterObserver(observer, 1);
	wdInside.SetMeasurements(25, 0.7, 760);
	wdOutside.SetMeasurements(4, 0.8, 761);
	wdInside.SetMeasurements(23, 0.7, 760);

	CHECK(stringStream.str() == "Indoors Outside Indoors ");
}

TEST_CASE("removing yourself from the observer list in the Update method")
{
	CWeatherData wd(SensorType::External);
	CRemoveDisplay removeDisplay(wd);
	wd.RegisterObserver(removeDisplay, 1);
	wd.SetMeasurements(5, 0.9, 764);
	wd.SetMeasurements(4, 0.8, 763);
	wd.RemoveObserver(removeDisplay);
}

TEST_CASE("priority notification to observers")
{
	CWeatherData wd(SensorType::Internal);
	std::stringstream stringStream;
	ObserverWithPriorityOutput observerFirst(stringStream, "Hel");
	ObserverWithPriorityOutput observerSecond(stringStream, "lo ");
	ObserverWithPriorityOutput observerThird(stringStream, "wor");
	ObserverWithPriorityOutput observerFourth(stringStream, "ld!");

	wd.RegisterObserver(observerThird, 2);
	wd.RegisterObserver(observerSecond, 3);
	wd.RegisterObserver(observerFirst, 4);
	wd.RegisterObserver(observerFourth, 1);

	wd.SetMeasurements(5, 0.9, 764);
	CHECK(stringStream.str() == "Hello world!");
}