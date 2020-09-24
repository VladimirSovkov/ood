#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include "Observer.h"
#define _USE_MATH_DEFINES 
#include <math.h>

struct SWindInfo
{
	double speed = 0;
	double direction = 0;
};

struct SWeatherInfoOutside
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	SWindInfo wind;
};

struct SWeatherInfoInside
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

class CDisplayInfoOutside : public IObserver<SWeatherInfoOutside>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfoOutside const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "Current wind speed " << data.wind.speed << std::endl;
		std::cout << "Current wind direction " << data.wind.direction << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

class CDisplayInfoInside : public IObserver<SWeatherInfoInside>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfoInside const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

class CMeanWindDirection
{
public:
	void Update(double value)
	{
		m_x += std::cos(value * M_PI / 180);
		m_y += std::sin(value * M_PI / 180);
	}

	std::string ToString()
	{
		double value = std::atan2(m_y, m_x) * 180 / M_PI;
		value = round(value * 100) / 100;
		if (value < 0)
			value += 360;
		return "Mean wind: " + std::to_string(value);
	}
private:
	double m_x = 0;
	double m_y = 0;
};

class CStatistics
{
public:
	void Update(double value)
	{
		if (m_min > value)
			m_min = value;
		if (m_max < value)
			m_max = value;
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

class CDisplayStatisticsOutside : public IObserver<SWeatherInfoOutside>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherInfoOutside const& data) override
	{
		m_temperatureStatistics.Update(data.temperature);
		m_humidityStatistics.Update(data.humidity);
		m_pressureStatistics.Update(data.pressure);
		m_windSpeedStatistics.Update(data.wind.speed);
		m_windDirectionStatistics.Update(data.wind.direction);

		std::cout << "Temperature: " << m_temperatureStatistics.ToString() << std::endl;
		std::cout << "Humidity: " << m_humidityStatistics.ToString()  << std::endl;
		std::cout << "Pressure: " << m_pressureStatistics.ToString() << std::endl;
		std::cout << "Wind: " << std::endl;
		std::cout << "\tSpeed: " << m_windSpeedStatistics.ToString() << std::endl;
		std::cout << "\tDirection: " << m_windDirectionStatistics.ToString() << std::endl;
		std::cout << "----------------" << std::endl;
	} 

	CStatistics m_temperatureStatistics;
	CStatistics m_humidityStatistics;
	CStatistics m_pressureStatistics;
	CStatistics m_windSpeedStatistics;
	CMeanWindDirection m_windDirectionStatistics;
};

class CDisplayStatisticsInside : public IObserver<SWeatherInfoInside>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherInfoInside const& data) override
	{
		m_temperatureStatistics.Update(data.temperature);
		m_humidityStatistics.Update(data.humidity);
		m_pressureStatistics.Update(data.pressure);

		std::cout << "Temperature: " << m_temperatureStatistics.ToString() << std::endl;
		std::cout << "Humidity: " << m_humidityStatistics.ToString() << std::endl;
		std::cout << "Pressure: " << m_pressureStatistics.ToString() << std::endl;
		std::cout << "----------------" << std::endl;
	}

	CStatistics m_temperatureStatistics;
	CStatistics m_humidityStatistics;
	CStatistics m_pressureStatistics;
};

class CWeatherDataOutside : public CObservable<SWeatherInfoOutside>
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

	//Информация о ветре {скорость (0..100), направление (0..360)}
	SWindInfo GetWindInfo()const 
	{
		return m_wind;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;
		m_wind.speed = windSpeed;
		m_wind.direction = windDirection;

		MeasurementsChanged();
		}
protected:
	SWeatherInfoOutside GetChangedData()const override
	{
		SWeatherInfoOutside info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		info.wind = GetWindInfo();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
	SWindInfo m_wind;
};

class CWeatherDataInside : public CObservable<SWeatherInfoInside>
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
	SWeatherInfoInside GetChangedData()const override
	{
		SWeatherInfoInside info;
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