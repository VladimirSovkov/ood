#pragma once
#include "IParagraph.h"
#include "CommandFunc.h"
#include "CReplaceTextCommand.h"

class CParagraph : public IParagraph
{
public:
	CParagraph(CommandFunc commandFunc, std::string const& text);

	std::string GetText() const override;
	void SetText(const std::string& text) override;

private:
	CommandFunc m_commandFunc;
	std::string m_text;
};

