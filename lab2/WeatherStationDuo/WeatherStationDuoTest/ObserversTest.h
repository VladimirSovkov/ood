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
	void Update(SWeatherInfo const& data) override
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
	void Update(SWeatherInfo const& data) override
	{
		m_outStream << m_data;
	}

	std::ostream& m_outStream;
	std::string m_data;
};

class ObserverWithTypeSensorOutput : public IObserver<SWeatherInfo>
{
public:
	ObserverWithTypeSensorOutput(std::ostream& outStream)
		:m_outStream(outStream)
	{
	}

private:
	void Update(SWeatherInfo const& data) override
	{
		if (data.sensorType == SensorType::Internal)
		{
			m_outStream << "Indoors ";
		}
		else if (data.sensorType == SensorType::External)
		{
			m_outStream << "Outside ";
		}
	}

	std::ostream& m_outStream;
};
