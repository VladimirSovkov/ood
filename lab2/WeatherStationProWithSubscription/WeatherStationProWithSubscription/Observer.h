#pragma once
#include <map>
#include <set>
#include <functional>

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T, typename Event>
class IObserver
{
public:
	virtual void Update(T const& data, Event changedEvent) = 0;
	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T, typename Event>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T, Event>& observer, unsigned int priority) = 0;
	virtual void NotifyObservers(std::set<Event> const& changedEvents) = 0;
	virtual void RemoveObserver(IObserver<T, Event>& observer) = 0;
	virtual void AddEvent(IObserver<T, Event>& observer, Event event) = 0;
	virtual void DeleteEvent(IObserver<T, Event>& observer, Event event) = 0;
};

// Реализация интерфейса IObservable
template <class T, class Event>
class CObservable : public IObservable<T, Event>
{
public:
	typedef IObserver<T, Event> ObserverType;

	void RegisterObserver(ObserverType& observer, unsigned int priority) override
	{
		bool isFound = std::any_of(m_observers.begin(), m_observers.end(), [&](auto it)
			{return &observer == it.second; });
		if (!isFound)
		{
			m_observers.emplace(priority, &observer);
			m_events.emplace(&observer, std::set<Event>{});
		}
	}

	void NotifyObservers(std::set<Event> const& changedEvents) override
	{
		T data = GetChangedData();
		for (auto it = m_observers.rbegin(); it != m_observers.rend(); ++it)
		{
			std::set<Event>eventSet = m_events[it->second];
			for (Event item : eventSet)
			{
				if (changedEvents.find(item) != changedEvents.end())
				{
					it->second->Update(data, item);
				}
			}
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		for (auto it = m_observers.begin(); it != m_observers.end(); it++)
		{
			if (it->second == &observer)
			{
				m_observers.erase(it);
				break;
			}
		}

		auto it = m_events.find(&observer);
		if (it != m_events.end())
		{
			m_events.erase(it);
		}
	}
	
	void AddEvent(ObserverType& observer, Event eventType) override
	{
		if (m_events.find(&observer) != m_events.end())
		{
			m_events[&observer].insert(eventType);
		}
	}
	
	void DeleteEvent(ObserverType& observer, Event eventType) override
	{
		if (m_events.find(&observer) != m_events.end())
		{
			m_events[&observer].erase(eventType);
		}
	}

protected:
	// Классы-наследники должны перегрузить данный метод, 
	// в котором возвращать информацию об изменениях в объекте
	virtual T GetChangedData()const = 0;

private:
	std::multimap<int, ObserverType*> m_observers;
	std::map<ObserverType*, std::set<Event>> m_events;
};
