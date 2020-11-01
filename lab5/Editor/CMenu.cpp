#include "CMenu.h"

CMenu::CMenu(std::istream& input, std::ostream& output)
	:m_istream(input)
	, m_ostream(output)
{
}

void CMenu::AddItem(const std::string& shortcut, const std::string& description, const Command& command)
{
	m_items.emplace_back(Item(shortcut, description, command));
}

void CMenu::Run()
{
	ShowInstructions();

	std::string command;
	while ((m_ostream << ">")
		&& getline(m_istream, command)
		&& ExecuteCommand(command))
	{
	}
}

void CMenu::ShowInstructions() const
{
	m_ostream << "Commands list:\n";
	for (auto& item : m_items)
	{
		m_ostream << " " << item.shortcut << ": " << item.description << "\n";
	}
}

void CMenu::Exit()
{
	m_exit = true;
}

bool CMenu::ExecuteCommand(const std::string& command)
{
	std::istringstream iss(command);
	std::string name;
	iss >> name;

	m_exit = false;
	auto it = std::find_if(m_items.begin(), m_items.end(), [&](const Item& item) {
		return item.shortcut == name;
		});
	if (it != m_items.end())
	{
		it->command(iss);
	}
	else
	{
		m_ostream << "Unknown command\n";
	}
	return !m_exit;
}

 