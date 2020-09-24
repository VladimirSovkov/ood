#include "WeatherData.h"
#include <map>

int main()
{
	CWeatherData wd;
	CDisplay display;
	CStatsDisplay statsDisplay;
	
	wd.RegisterObserver(display, 2);
	wd.RegisterObserver(statsDisplay, 1);

	wd.DeleteEvent(statsDisplay, Event::Humidity);
	wd.AddEvent(display, Event::Temperature);
	wd.AddEvent(display, Event::Pressure);
	wd.AddEvent(statsDisplay, Event::Pressure);

	wd.SetMeasurements(3, 0.7, 760, 6, 21);
	wd.SetMeasurements(4, 0.8, 761, 5, 16);
	wd.SetMeasurements(4, 0.7, 762, 6, 16);

	wd.DeleteEvent(statsDisplay, Event::Pressure);
	wd.SetMeasurements(-10, 0.8, 761, 3, 349);
	
	wd.AddEvent(display, Event::Temperature);
	wd.RemoveObserver(statsDisplay);

	wd.RemoveObserver(display);
	wd.SetMeasurements(500, 0.7, 761, 3, 349);

	return 0;
}