#pragma once
#include <istream>
#include "CCommandMenu.h"
#include "GumBallMachineWithState.h"
#include "GumBallMachineWithState.h"
using namespace with_state;

class CGumballMachineMenu
{
public:
	CGumballMachineMenu();

	void Run();

private:
	typedef void (CGumballMachineMenu::* Command)(std::istream& input);

	void AddCommand(const std::string& shortcut, const std::string& description, Command handler);

	void EjectQuarter(std::istream& input);
	void InsertQuarter(std::istream& input);
	void Refill(std::istream& input);
	void ToString(std::istream& input);
	void TurnCrank(std::istream& input);

	std::unique_ptr<CGumballMachine> m_gumballMachine;
	CCommandMenu m_menu;
};

