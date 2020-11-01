#include "CParagraph.h"
CParagraph::CParagraph(CommandFunc commandFunc, std::string const& text)
    :m_commandFunc(commandFunc)
    , m_text(text)
{
}

std::string CParagraph::GetText() const
{
    return m_text;
}

void CParagraph::SetText(const std::string& text)
{
    m_commandFunc(std::make_unique<CReplaceTextCommand>(m_text, text));
}
