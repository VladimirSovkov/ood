#pragma once
#include <sstream>
#include <iostream>

namespace with_state
{


struct IState
{
	virtual void InsertQuarter() = 0;
	virtual void EjectQuarter() = 0;
	virtual void TurnCrank() = 0;
	virtual void Dispense() = 0;
	virtual std::string ToString()const = 0;
	virtual ~IState() = default;
};

struct IGumballMachine
{
	virtual void ReleaseBall() = 0;
	virtual unsigned GetBallCount()const = 0;
	virtual unsigned GetQuarterCount()const = 0;
	virtual void AddQuarter() = 0;
	virtual void RemoveQuarter() = 0;
	virtual void EjectQuarters() = 0;

	virtual void SetSoldOutState() = 0;
	virtual void SetNoQuarterState() = 0;
	virtual void SetSoldState() = 0;
	virtual void SetHasQuarterState() = 0;
	virtual void SetMaxQuarterCountState() = 0;

	virtual ~IGumballMachine() = default;
};

class CSoldState : public IState
{
public:
	CSoldState(IGumballMachine & gumballMachine)
		:m_gumballMachine(gumballMachine)
	{}
	void InsertQuarter() override
	{
		std::cout << "Please wait, we're already giving you a gumball\n";
	}
	void EjectQuarter() override
	{
		std::cout << "Please wait, we're already giving you a gumball\n";
	}
	void TurnCrank() override
	{
		std::cout << "Turning twice doesn't get you another gumball\n";
	}
	void Dispense() override
	{
		m_gumballMachine.ReleaseBall();
		m_gumballMachine.RemoveQuarter();
		if (m_gumballMachine.GetBallCount() == 0)
		{
			std::cout << "Oops, out of gumballs\n";
			m_gumballMachine.SetSoldOutState();
		}
		else if(m_gumballMachine.GetQuarterCount() > 0)
		{
			m_gumballMachine.SetHasQuarterState();
		}
		else
		{
			m_gumballMachine.SetNoQuarterState();
		}
	}
	std::string ToString() const override
	{
		return "delivering a gumball";
	}
private:
	IGumballMachine & m_gumballMachine;
};

class CSoldOutState : public IState
{
public:
	CSoldOutState(IGumballMachine & gumballMachine)
		:m_gumballMachine(gumballMachine)
	{}

	void InsertQuarter() override
	{
		std::cout << "You can't insert a quarter, the machine is sold out\n";
	}
	void EjectQuarter() override
	{
		if (m_gumballMachine.GetQuarterCount())
		{
			m_gumballMachine.EjectQuarters();
		}
		else
		{
			std::cout << "You can't eject, you haven't inserted a quarter yet\n";
		}
	}
	void TurnCrank() override
	{
		std::cout << "You turned but there's no gumballs\n";
	}
	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}
	std::string ToString() const override
	{
		return "sold out";
	}
private:
	IGumballMachine & m_gumballMachine;
};

class CHasQuarterState : public IState
{
public:
	const unsigned MAX_QUARTERS_COUNT = 5;

	CHasQuarterState(IGumballMachine & gumballMachine)
		:m_gumballMachine(gumballMachine)
	{}

	void InsertQuarter() override
	{
		m_gumballMachine.AddQuarter();
		std::cout << "You inserted another quarter\n";
		if (m_gumballMachine.GetQuarterCount() >= MAX_QUARTERS_COUNT)
		{
			m_gumballMachine.SetMaxQuarterCountState();
		}
	}
	void EjectQuarter() override
	{
		m_gumballMachine.EjectQuarters();
		m_gumballMachine.SetNoQuarterState();
	}
	void TurnCrank() override
	{
		std::cout << "You turned...\n";
		m_gumballMachine.SetSoldState();
	}
	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}
	std::string ToString() const override
	{
		return "waiting for turn of crank or insert of quarter";
	}
private:
	IGumballMachine & m_gumballMachine;
};

class CNoQuarterState : public IState
{
public:
	CNoQuarterState(IGumballMachine & gumballMachine)
		: m_gumballMachine(gumballMachine)
	{}

	void InsertQuarter() override
	{
		m_gumballMachine.AddQuarter();
		std::cout << "You inserted a quarter\n";
		m_gumballMachine.SetHasQuarterState();
	}
	void EjectQuarter() override
	{
		std::cout << "You haven't inserted a quarter\n";
	}
	void TurnCrank() override
	{
		std::cout << "You turned but there's no quarter\n";
	}
	void Dispense() override
	{
		std::cout << "You need to pay first\n";
	}
	std::string ToString() const override
	{
		return "waiting for quarter";
	}
private:
	IGumballMachine & m_gumballMachine;
};

class CMaxQuarterCountState : public IState
{
public:
	CMaxQuarterCountState(IGumballMachine& gumballMachine)
		:m_gumballMachine(gumballMachine)
	{}

	void InsertQuarter() override
	{
		std::cout << "You can't insert another quarter\n";
	}

	void EjectQuarter() override
	{
		m_gumballMachine.EjectQuarters();
		m_gumballMachine.SetNoQuarterState();
	}
	void TurnCrank() override
	{
		std::cout << "You turned...\n";
		m_gumballMachine.SetSoldState();
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	std::string ToString()const override
	{
		return "waiting for turn of crank with maximum quarters";
	}

private:
	IGumballMachine& m_gumballMachine;
};

class CGumballMachine : private IGumballMachine
{
public:
	CGumballMachine(unsigned numBalls)
		: m_soldState(*this)
		, m_soldOutState(*this)
		, m_noQuarterState(*this)
		, m_hasQuarterState(*this)
		, m_maxQuarterCountState(*this)
		, m_state(&m_soldOutState)
		, m_count(numBalls)
	{
		if (m_count > 0)
		{
			m_state = &m_noQuarterState;
		}
	}

	void EjectQuarter()
	{
		m_state->EjectQuarter();
	}

	void InsertQuarter()
	{
		m_state->InsertQuarter();
	}

	void TurnCrank()
	{
		m_state->TurnCrank();
		m_state->Dispense();
	}

	std::string ToString()const
	{
		std::ostringstream ss;
		ss << "Mighty Gumball, Inc.\n"
			<< "C++-enabled Standing Gumball Model #2016 (with state)\n"
			<< "Inventory: " << m_count << " gumball" << (m_count != 1 ? "s, " : ", ")
			<< m_quarterCount << " quarter" << (m_quarterCount != 1 ? "s\n" : "\n")
			<< "Machine is " << m_state->ToString() << std::endl;
		return ss.str();
	}

private:
	unsigned GetBallCount() const override
	{
		return m_count;
	}

	virtual void ReleaseBall() override
	{
		if (m_count != 0)
		{
			std::cout << "A gumball comes rolling out the slot...\n";
			--m_count;
		}
	}

	void AddQuarter() override
	{
		++m_quarterCount;
	}

	unsigned GetQuarterCount() const override
	{
		return m_quarterCount;
	}

	void RemoveQuarter() override
	{
		if (m_quarterCount)
		{
			--m_quarterCount;
		}
	}

	void EjectQuarters() override
	{
		std::cout << m_quarterCount << " quarters rolling out of the slot...\n";
		m_quarterCount = 0;
	}

	void SetSoldOutState() override
	{
		m_state = &m_soldOutState;
	}
	void SetNoQuarterState() override
	{
		m_state = &m_noQuarterState;
	}
	void SetSoldState() override
	{
		m_state = &m_soldState;
	}
	void SetHasQuarterState() override
	{
		m_state = &m_hasQuarterState;
	}

	void SetMaxQuarterCountState() override
	{
		m_state = &m_maxQuarterCountState;
	}

private:
	unsigned m_count = 0;
	unsigned m_quarterCount = 0;
	CSoldState m_soldState;
	CSoldOutState m_soldOutState;
	CNoQuarterState m_noQuarterState;
	CHasQuarterState m_hasQuarterState;
	CMaxQuarterCountState m_maxQuarterCountState;
	IState * m_state;
};

}
