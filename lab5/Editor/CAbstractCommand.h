#pragma once
#include "ICommand.h"

class CAbstractCommand : public ICommand
{
public:
	virtual ~CAbstractCommand() = default;

	void Execute() override;
	void Unexecute() override;

protected:
	virtual void DoExecute() = 0;
	virtual void DoUnexecute() = 0;

private:
	bool m_isExecuted = false;
};