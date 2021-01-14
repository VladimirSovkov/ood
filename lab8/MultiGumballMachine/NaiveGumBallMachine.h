#pragma once
#include <iostream>
#include <sstream>

namespace naive
{
class CGumballMachine
{
public:
	enum class State
	{
		SoldOut,			// Жвачка закончилась
		NoQuarter,			// Нет монетки
		HasQuarter,			// Есть монетка
		Sold,				// Монетка выдана
		MaxQuarterCount,	// Количество монет максимально
	};

	CGumballMachine(unsigned count)
		: m_count(count)
		, m_state(count > 0 ? State::NoQuarter : State::SoldOut)
	{
	}

	void InsertQuarter()
	{
		using namespace std;
		switch (m_state)
		{
		case State::SoldOut:
			cout << "You can't insert a quarter, the machine is sold out\n";
			break;
		case State::NoQuarter:
			++m_quarterCount;
			cout << "You inserted a quarter\n";
			m_state = State::HasQuarter;
			break;
		case State::HasQuarter:
			++m_quarterCount;
			cout << "You inserted another quarter\n";
			if (m_quarterCount >= 5)
			{
				m_state = State::MaxQuarterCount;
			}
			break;
		case State::Sold:
			cout << "Please wait, we're already giving you a gumball\n";
			break;
		case State::MaxQuarterCount:
			cout << "You can't insert another quarter\n";
			break;
		}
	}

	void EjectQuarter()
	{
		using namespace std;
		switch (m_state)
		{
		case State::HasQuarter:
		case State::MaxQuarterCount:
			cout << m_quarterCount << " quarters rolling out of the slot...\n";
			m_quarterCount = 0;
			m_state = State::NoQuarter;
			break;
		case State::NoQuarter:
			cout << "You haven't inserted a quarter\n";
			break;
		case State::Sold:
			cout << "Please wait, we're already giving you a gumball\n";
			break;
		case State::SoldOut:
			if (m_quarterCount)
			{
				cout << m_quarterCount << " quarters comes rolling out the slot...\n";
				m_quarterCount = 0;
			}
			else
			{
				cout << "You can't eject, you haven't inserted a quarter yet\n";
			}
			break;
		}
	}

	void TurnCrank()
	{
		using namespace std;
		switch (m_state)
		{
		case State::SoldOut:
			cout << "You turned but there's no gumballs\n";
			break;
		case State::NoQuarter:
			cout << "You turned but there's no quarter\n";
			break;
		case State::HasQuarter:
		case State::MaxQuarterCount:
			cout << "You turned...\n";
			m_state = State::Sold;
			Dispense();
			break;
		case State::Sold:
			cout << "Turning twice doesn't get you another gumball\n";
			break;
		}
	}

	std::string ToString()const
	{
		std::string state =
			(m_state == State::SoldOut)			? "sold out" :
			(m_state == State::NoQuarter)		? "waiting for quarter" :
			(m_state == State::HasQuarter)		? "waiting for turn of crank or insert of quarter" :
			(m_state == State::MaxQuarterCount) ? "waiting for turn of crank with maximum count quarters"
												: "delivering a gumball";
		std::ostringstream ss;
		ss << "Mighty Gumball, Inc.\n"
			<< "C++-enabled Standing Gumball Model #2016\n"
			<< "Inventory: " << m_count << " gumball" << (m_count != 1 ? "s, " : ", ")
			<< m_quarterCount << " quarter" << (m_quarterCount != 1 ? "s\n" : "\n")
			<< "Machine is " << state << std::endl;
		return ss.str();
	}

private:
	void Dispense()
	{
		using namespace std;
		switch (m_state)
		{
		case State::Sold:
			cout << "A gumball comes rolling out the slot...\n";
			--m_count;
			--m_quarterCount;
			if (m_count == 0)
			{
				cout << "Oops, out of gumballs\n";
				m_state = State::SoldOut;
			}
			else if (m_quarterCount > 0)
			{
				m_state = State::HasQuarter;
			}
			else
			{
				m_state = State::NoQuarter;
			}
			break;
		case State::NoQuarter:
			cout << "You need to pay first\n";
			break;
		case State::SoldOut:
		case State::HasQuarter:
		case State::MaxQuarterCount:
			cout << "No gumball dispensed\n";
			break;
		}
	}

	unsigned m_quarterCount = 0;
	unsigned m_count;			// Количество шариков
	State m_state = State::SoldOut;
};
}