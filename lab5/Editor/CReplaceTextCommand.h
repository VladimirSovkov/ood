#pragma once
#include "CAbstractCommand.h"
#include "IParagraph.h"
#include <string>

class CReplaceTextCommand : public CAbstractCommand
{
public:
	CReplaceTextCommand(std::string& textContainer, const std::string& nextText);

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	std::string& m_textContainer;
	std::string m_nextText;
};

