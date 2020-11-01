#include "CHistory.h"

bool CHistory::CanUndo() const
{
    return m_nextCommandIndex > 0;
}

bool CHistory::CanRedo() const
{
    return m_nextCommandIndex < m_commands.size();
}

void CHistory::Undo()
{
	if (CanUndo())
	{
		m_commands[m_nextCommandIndex - 1]->Unexecute();
		--m_nextCommandIndex;
	}
}

void CHistory::Redo()
{
	if (CanRedo())
	{
		m_commands[m_nextCommandIndex]->Execute();
		++m_nextCommandIndex;
	}
}

void CHistory::AddAndExecuteCommand(std::unique_ptr<ICommand>&& commandPtr)
{
	if (m_nextCommandIndex < MAX_HISTORY_QUEUE_SIZE)
	{
		commandPtr->Execute();
		m_commands.resize(m_nextCommandIndex);
		m_commands.push_back(std::move(commandPtr));
		++m_nextCommandIndex;
	}
	else
	{
		m_commands.push_back(std::move(commandPtr));
		m_commands.erase(m_commands.begin());
	}
}