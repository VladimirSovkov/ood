#pragma once
#include "CAbstractCommand.h"
#include "CDocumentItem.h"
#include <optional>

class CInsertDocumentItemCommand : public CAbstractCommand
{
public:
	CInsertDocumentItemCommand(CDocumentItem documentItem, std::vector<CDocumentItem>& documentItemArr, std::optional<size_t> position = std::nullopt);
	~CInsertDocumentItemCommand() = default;

protected:
	virtual void DoExecute() override;
	virtual void DoUnexecute() override;

private:
	CDocumentItem m_documentItem;
	std::vector<CDocumentItem>& m_documentItemArr;
	std::optional<size_t> m_position;
};