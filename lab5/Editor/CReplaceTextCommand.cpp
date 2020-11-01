#include "CReplaceTextCommand.h"

CReplaceTextCommand::CReplaceTextCommand(std::string& textContainer, const std::string& nextText)
	:m_textContainer(textContainer)
	, m_nextText(nextText)
{
}

void CReplaceTextCommand::DoExecute()
{
	std::swap(m_textContainer, m_nextText);
}

void CReplaceTextCommand::DoUnexecute()
{
	std::swap(m_textContainer, m_nextText);
}
