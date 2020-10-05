#include "WeatherData.h"
#include <map>

int main()
{
	CWeatherData wdInside(SensorType::Internal);
	CWeatherData wdOutside(SensorType::External);

	CDisplay display;
	wdInside.RegisterObserver(display, 2);
	CStatsDisplay statsDisplay;
	wdInside.RegisterObserver(statsDisplay, 1);
	wdOutside.RegisterObserver(statsDisplay, 1);

	wdInside.SetMeasurements(3, 0.7, 760);
	wdOutside.SetMeasurements(4, 0.8, 761);
	wdOutside.RemoveObserver(statsDisplay);

	wdInside.SetMeasurements(10, 0.8, 761);
	wdInside.SetMeasurements(-10, 0.8, 761);
	return 0;
}