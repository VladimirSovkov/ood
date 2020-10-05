#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include "Observer.h"
#include <iostream>

enum SensorType
{
	Internal,
	External
};

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	SensorType sensorType;
};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	CWeatherData(SensorType type)
		: m_type(type)
	{
	}

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
		info.sensorType = m_type;
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
	SensorType m_type;
};

class CDisplay : public IObserver<SWeatherInfo>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data) override
	{
		if (data.sensorType == SensorType::Internal)
		{
			std::cout << "Indoors:" << std::endl;
		}
		else if (data.sensorType == SensorType::External)
		{
			std::cout << "Outside:" << std::endl;
		}

		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
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
	CStatsDisplay()
	{
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherInfo const& data) override
	{
		if (data.sensorType == SensorType::Internal)
		{
			std::cout << "Indoors:" << std::endl;
			m_weatherStatisticsInside.Update(data);
			m_weatherStatisticsInside.PrintStatistics();
			}
		else if (data.sensorType == SensorType::External)
		{
			std::cout << "Outside:" << std::endl;
			m_weatherStatisticsOutside.Update(data);
			m_weatherStatisticsOutside.PrintStatistics();
		}
	} 

	WeatherDataStatistics m_weatherStatisticsInside;
	WeatherDataStatistics m_weatherStatisticsOutside;
};

