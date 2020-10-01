#pragma once
#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage
{
public:
	CBeverage(const std::string& description)
		:m_description(description)
	{}

	std::string GetDescription()const override final
	{
		return m_description;
	}
protected:
	std::string m_description;
};

// Кофе
class CCoffee : public CBeverage
{
public:
	CCoffee(const std::string& description = "Coffee")
		:CBeverage(description)
	{}

	double GetCost() const override
	{
		return 60;
	}
};

enum class CappuccinoPortion
{
	Standard,
	Double
};

// Капуччино
class CCappuccino : public CCoffee
{
public:
	CCappuccino(CappuccinoPortion portion = CappuccinoPortion::Standard)
		:CCoffee("Cappuccino")
	{
		switch (portion)
		{
		case CappuccinoPortion::Double:
			m_description = "Double cappuccino";
			m_cost = 120;
			break;
		default:
			m_description = "Standard cappuccino";
			m_cost = 80;
			break;
		}
	}

	double GetCost() const override
	{
		return m_cost;
	}

private:
	double m_cost;
};

enum class LattePortion
{
	Standard,
	Double
};

// Латте
class CLatte : public CCoffee
{
public:
	CLatte(LattePortion portion = LattePortion::Standard)
		:CCoffee("Latte")
	{

		switch (portion)
		{
		case LattePortion::Double:
			m_description = "Double latte";
			m_cost = 130;
			break;
		default:
			m_description = "Standard latte";
			m_cost = 90;
			break;
		}
	}

	double GetCost() const override
	{
		return m_cost;
	}
private: 
	double m_cost;
};

// Чай
class CTea : public CBeverage
{
public:
	CTea(const std::string& description = "Tea")
		:CBeverage(description)
	{}

	double GetCost() const override
	{
		return 30;
	}
};

//зеленый чай
class CGreenTea : public CTea
{
public:
	CGreenTea()
		:CTea("GreenTea")
	{}

	double GetCost() const override
	{
		return 30;
	}
};

//черный чай 
class CBlackTea : public CTea
{
public:
	CBlackTea()
		:CTea("Black tea")
	{}

	double GetCost() const override
	{
		return 30;
	}
};

//белый чай 
class CWhiteTea : public CTea
{
public:
	CWhiteTea()
		:CTea("White tea")
	{}

	double GetCost() const override
	{
		return 30;
	}
};

//желтый чай
class CYellowTea : public CTea
{
public:
	CYellowTea()
		:CTea("Yellow tea")
	{}

	double GetCost() const override
	{
		return 30;
	}
};

enum MilkshakeType
{
	Small,
	Medium,
	Large
};

// Молочный коктейль
class CMilkshake : public CBeverage
{
public:
	CMilkshake(MilkshakeType type = MilkshakeType::Medium)
		:CBeverage("Milkshake")
	{
		switch (type)
		{
		case Small:
			m_description = "Small milkshake";
			m_cost = 50;
			break;
		case Large:
			m_description = "Large milkshake";
			m_cost = 80;
			break;
		default:
			m_description = "Medium milkshake";
			m_cost = 60;
			break;
		}
	}

	double GetCost() const override
	{
		return m_cost;
	}

private: 
	double m_cost;

};