#pragma once
#include "ICommand.h"
#include <vector>
#include <memory>

class CHistory
{
public:
	bool CanUndo() const;
	bool CanRedo() const;

	void Undo();
	void Redo();
	void AddAndExecuteCommand(std::unique_ptr<ICommand>&& commandPtr);

private:
	const size_t MAX_HISTORY_QUEUE_SIZE = 10;

	std::vector<std::unique_ptr<ICommand>> m_commands;
	size_t m_nextCommandIndex = 0;
};

