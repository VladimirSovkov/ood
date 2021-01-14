#pragma once
#include <iostream>
#include <functional>
#include <map>


class CCommandMenu
{
public:
	typedef std::function<void(std::istream& args)> Command;
	CCommandMenu(std::istream& input, std::ostream& output);
	
	void AddCommand(const std::string& title, const std::string& description, Command command);
	void Run();
	void ShowCommands();
	void Exit();

private:
	struct Item
	{
		std::string description;
		CCommandMenu::Command command;
	};

	std::istream& m_input;
	std::ostream& m_output;
	std::map<std::string, Item> m_commandMap;
	bool m_isEnd = false;

	void ExecuteCommand(const std::string& commandLine);
};

