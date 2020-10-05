#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include "Observer.h"
#define _USE_MATH_DEFINES 
#include <math.h>

enum class Event
{
	Humidity,
	Temperature,
	Pressure,
	WindSpeed,
	WindDirection,
};

struct SWindInfo
{
	double speed = 0;
	double direction = 0;
};

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	SWindInfo wind;
};

class CDisplay : public IObserver<SWeatherInfo, Event>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data, Event changedEvent) override
	{
		switch (changedEvent)
		{
		case Event::Temperature:
			std::cout << "Current Temp " << data.temperature << std::endl;
			break;
		case Event::Humidity:
			std::cout << "Current Hum " << data.humidity << std::endl;
			break;
		case Event::Pressure:
			std::cout << "Current Pressure " << data.pressure << std::endl;
			break;
		case Event::WindSpeed:
			std::cout << "Current wind speed " << data.wind.speed << std::endl;
			break;
		case Event::WindDirection:
			std::cout << "Current wind direction " << data.wind.direction << std::endl;
			break;
		default:
			break;
		}
	}
};

class MeanWindDirection
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

class CStatsDisplay : public IObserver<SWeatherInfo, Event>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherInfo const& data, Event changedEvent) override
	{
		switch (changedEvent)
		{
		case Event::Temperature:
			m_temperatureStatistics.Update(data.temperature);
			std::cout << "Temperature: " << m_temperatureStatistics.ToString() << std::endl;
			break;
		case Event::Humidity:
			m_humidityStatistics.Update(data.humidity);
			std::cout << "Humidity: " << m_humidityStatistics.ToString() << std::endl;
			break;
		case Event::Pressure:
			m_pressureStatistics.Update(data.pressure);
			std::cout << "Pressure: " << m_pressureStatistics.ToString() << std::endl;
			break;
		case Event::WindSpeed:
			m_windSpeedStatistics.Update(data.wind.speed);
			std::cout << "\tSpeed: " << m_windSpeedStatistics.ToString() << std::endl;
			break;
		case Event::WindDirection:
			m_windDirectionStatistics.Update(data.wind.direction);
			std::cout << "\tDirection: " << m_windDirectionStatistics.ToString() << std::endl;
			break;
		default:
			break;
		}
	} 

	Statistics m_temperatureStatistics;
	Statistics m_humidityStatistics;
	Statistics m_pressureStatistics;
	Statistics m_windSpeedStatistics;
	MeanWindDirection m_windDirectionStatistics;
};

class CWeatherData : public CObservable<SWeatherInfo, Event>
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

	void MeasurementsChanged(std::set<Event> const& changedEvents)
	{
		NotifyObservers(changedEvents);
	}

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		std::set<Event> changedEvents = GetChangedEvents(temp, humidity, pressure, windSpeed, windDirection);
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;
		m_wind.speed = windSpeed;
		m_wind.direction = windDirection;

		MeasurementsChanged(changedEvents);
		}
protected:
	std::set<Event> GetChangedEvents(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		std::set<Event> changedEvents;
		if (m_humidity != humidity)
			changedEvents.insert(Event::Humidity);
		if (m_temperature != temp)
			changedEvents.insert(Event::Temperature);
		if (m_pressure != pressure)
			changedEvents.insert(Event::Pressure);
		if (m_wind.speed != windSpeed)
			changedEvents.insert(Event::WindSpeed);
		if (m_wind.direction != windDirection)
			changedEvents.insert(Event::WindDirection);
		return changedEvents;
	}

	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
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