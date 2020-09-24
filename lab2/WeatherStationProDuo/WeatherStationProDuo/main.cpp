#include "WeatherData.h"
#include <map>

int main()
{
	CWeatherDataInside wdInside;
	CWeatherDataOutside wdOutside;

	CDisplayInfoInside displayInside;
	CDisplayInfoOutside displayOutside;
	CDisplayStatisticsInside displayStatisticInside;
	CDisplayStatisticsOutside displayStatisticOutside;
	
	wdInside.RegisterObserver(displayInside, 2);
	wdInside.RegisterObserver(displayStatisticInside, 1);
	wdOutside.RegisterObserver(displayOutside, 2);
	wdOutside.RegisterObserver(displayStatisticOutside, 1);

	wdInside.SetMeasurements(3, 0.7, 760);
	wdOutside.SetMeasurements(4, 0.8, 761, 5, 16);
	wdInside.RemoveObserver(displayStatisticInside);
	wdOutside.RemoveObserver(displayStatisticOutside);
	wdOutside.SetMeasurements(10, 0.8, 761, 4, 10);
	wdInside.SetMeasurements(-10, 0.8, 761);

	return 0;
}