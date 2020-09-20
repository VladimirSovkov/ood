#pragma once
#include "../WeatherStation/Observer.h"
#include "../WeatherStation/WeatherData.h"

typedef IObservable<SWeatherInfo> ObserverType;

class CRemoveDisplay : public IObserver<SWeatherInfo>
{
public :
	CRemoveDisplay(ObserverType& observer)
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

class ObserverWithStringStream : public IObserver<SWeatherInfo>
{
public: 
	ObserverWithStringStream(std::ostream& outStream, std::string const& data)
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