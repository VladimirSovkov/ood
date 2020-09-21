#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include "Observer.h"

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	// Температура в градусах Цельсия
	double GetTemperature()const
	{
		return m_temperature;
	}
	// Относительная влажность (0...100)
	double GetHumidity()const
	{
		return m_humidity;
	}
	// Атмосферное давление (в мм.рт.ст)
	double GetPressure()const
	{
		return m_pressure;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;

		MeasurementsChanged();
	}
protected:
	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};

class CDisplay : public IObserver<SWeatherInfo>
{
public:
	CDisplay(CWeatherData const& indicatorInside, CWeatherData const& indicatorOutside)
		:m_indicatorInside(indicatorInside),
		m_indicatorOutside(indicatorOutside)
	{
	}
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data, IObservable<SWeatherInfo> const& observable) override
	{
		if (std::addressof(m_indicatorInside) == std::addressof(observable))
		{
			std::cout << "Indoors:" << std::endl;
		}
		else if(std::addressof(m_indicatorOutside) == std::addressof(observable))
		{
			std::cout << "outside:" << std::endl;
		}

		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}

	CWeatherData const& m_indicatorInside;
	CWeatherData const& m_indicatorOutside;
};

class Statistics
{
public:
	void Update(double value)
	{
		if (m_min > value)
		{
			m_min = value;
		}
		if (m_max < value)
		{
			m_max = value;
		}
		m_acc += value;
		++m_countAcc;
	}

	std::string ToString()
	{
		return "Max: " + std::to_string(m_max) + "; " + 
			"Min: " + std::to_string(m_min) + "; " + 
			"Average: " + std::to_string(m_acc / m_countAcc) + ".";
	}

private:
	double m_min = std::numeric_limits<double>::infinity();
	double m_max = -std::numeric_limits<double>::infinity();
	double m_acc = 0;
	unsigned m_countAcc = 0;
};

class WeatherDataStatistics
{
public:
	void Update(SWeatherInfo const& data)
	{
		m_temperatureStatistics.Update(data.temperature);
		m_humidityStatistics.Update(data.humidity);
		m_pressureStatistics.Update(data.pressure);
	}

	void PrintStatistics()
	{
		std::cout << "Temperature: " << m_temperatureStatistics.ToString() << std::endl;
		std::cout << "Humidity: " << m_humidityStatistics.ToString()  << std::endl;
		std::cout << "Pressure: " << m_pressureStatistics.ToString() << std::endl;
	}

private:
	Statistics m_temperatureStatistics;
	Statistics m_humidityStatistics;
	Statistics m_pressureStatistics;
};

class CStatsDisplay : public IObserver<SWeatherInfo>
{
public: 
	CStatsDisplay(CWeatherData const& indicatorInside, CWeatherData const& indicatorOutside)
		:m_indicatorInside(indicatorInside),
			m_indicatorOutside(indicatorOutside)
	{
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherInfo const& data, const IObservable<SWeatherInfo>& observable) override
	{
		if (std::addressof(m_indicatorInside) == std::addressof(observable))
		{
			std::cout << "Indoors:" << std::endl;
			m_weatherStatisticsInside.Update(data);
			m_weatherStatisticsInside.PrintStatistics();
			}
		else if (std::addressof(m_indicatorOutside) == std::addressof(observable))
		{
			std::cout << "Outside:" << std::endl;
			m_weatherStatisticsOutside.Update(data);
			m_weatherStatisticsOutside.PrintStatistics();
		}
	} 

	WeatherDataStatistics m_weatherStatisticsInside;
	WeatherDataStatistics m_weatherStatisticsOutside;

	CWeatherData const& m_indicatorInside;
	CWeatherData const& m_indicatorOutside;
};

