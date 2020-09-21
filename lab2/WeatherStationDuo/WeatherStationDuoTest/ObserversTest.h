#pragma once
#include "../WeatherStationDuo/Observer.h"
#include "../WeatherStationDuo/WeatherData.h"

typedef IObservable<SWeatherInfo> ObserverType;

class CRemoveDisplay : public IObserver<SWeatherInfo>
{
public :
	CRemoveDisplay (ObserverType& observer)
	{
		m_observer = &observer;
	}
private:
	void Update(SWeatherInfo const& data, IObservable<SWeatherInfo> const& observable) override
	{
		m_observer->RemoveObserver(*this);
	}

	ObserverType* m_observer;
};

class ObserverWithPriorityOutput : public IObserver<SWeatherInfo>
{
public: 
	ObserverWithPriorityOutput(std::ostream& outStream, std::string const& data)
		:m_outStream(outStream),
		m_data(data)
	{
	}

private:
	void Update(SWeatherInfo const& data, IObservable<SWeatherInfo> const& observable) override
	{
		m_outStream << m_data;
	}

	std::ostream& m_outStream;
	std::string m_data;
};

class ObserverWithTypeSensorOutput : public IObserver<SWeatherInfo>
{
public:
	ObserverWithTypeSensorOutput(CWeatherData const& indicatorInside, CWeatherData const& indicatorOutside, std::ostream& outStream)
		:m_outStream(outStream),
		m_indicatorInside(indicatorInside),
		m_indicatorOutside(indicatorOutside)
	{
	}

private:
	void Update(SWeatherInfo const& data, IObservable<SWeatherInfo> const& observable) override
	{
		if (std::addressof(m_indicatorInside) == std::addressof(observable))
		{
			m_outStream << "Indoors ";
		}
		else if (std::addressof(m_indicatorOutside) == std::addressof(observable))
		{
			m_outStream << "Outside ";
		}
	}

	std::ostream& m_outStream;
	CWeatherData const& m_indicatorInside;
	CWeatherData const& m_indicatorOutside;
};
