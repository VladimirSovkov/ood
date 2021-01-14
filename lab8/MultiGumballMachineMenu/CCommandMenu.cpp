#include "CCommandMenu.h"
#include <string>
#include <sstream>

CCommandMenu::CCommandMenu(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
}

void CCommandMenu::AddCommand(const std::string& title, const std::string& description, Command command)
{
	m_commandMap[title] = { description, command };
}

void CCommandMenu::Run()
{
	m_isEnd = false;

	ShowCommands();
	std::string commandLine;
	while (!m_isEnd && m_output << ">" && std::getline(m_input, commandLine))
	{
		try
		{
			ExecuteCommand(commandLine);
		}
		catch (const std::exception& e)
		{
			m_output << e.what() << std::endl;
		}
	}
}

void CCommandMenu::ShowCommands()
{
	for (const auto& item : m_commandMap)
	{
		m_output << item.first << ": " << item.second.description << std::endl;
	}
}

void CCommandMenu::Exit()
{
	m_isEnd = true;
}

void CCommandMenu::ExecuteCommand(const std::string& commandLine)
{
	std::istringstream ss(commandLine);

	std::string title;
	ss >> title;

	Item item;
	try
	{
		item = m_commandMap.at(title);
	}
	catch (...)
	{
		throw std::invalid_argument("Unknown command: " + title);
	}

	item.command(ss);
}
