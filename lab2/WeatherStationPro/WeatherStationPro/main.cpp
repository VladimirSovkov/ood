#include "WeatherData.h"
#include <map>

int main()
{
	CWeatherData wd;

	CDisplay display;
	wd.RegisterObserver(display, 2);
	CStatsDisplay statsDisplay;
	wd.RegisterObserver(statsDisplay, 1);

	wd.SetMeasurements(3, 0.7, 760, 6, 21);
	wd.SetMeasurements(4, 0.8, 761, 5, 16);
	wd.SetMeasurements(10, 0.8, 761, 4, 10);
	wd.SetMeasurements(-10, 0.8, 761, 3, 349);
	wd.RemoveObserver(statsDisplay);
	
	wd.SetMeasurements(-10, 0.8, 761, 15, 223);

	return 0;
}