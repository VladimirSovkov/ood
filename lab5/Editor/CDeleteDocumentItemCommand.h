#pragma once
#include "CAbstractCommand.h"
#include "CDocumentItem.h"

class CDeleteDocumentItemCommand : public CAbstractCommand
{
public:
	CDeleteDocumentItemCommand(std::vector<CDocumentItem>& documentItemArr, size_t position);

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	std::vector<CDocumentItem>& m_documentItemArr;
	size_t m_position;
	std::unique_ptr<CDocumentItem> m_documentItem;
};

