#include "CDeleteDocumentItemCommand.h"

CDeleteDocumentItemCommand::CDeleteDocumentItemCommand(std::vector<CDocumentItem>& documentItemArr, size_t position)
	:m_documentItemArr(documentItemArr)
	, m_position(position)
{
}

void CDeleteDocumentItemCommand::DoExecute()
{
	if (m_position >= m_documentItemArr.size())
	{
		throw std::out_of_range("Document item position is out of range");
	}
	m_documentItem = std::make_unique<CDocumentItem>(m_documentItemArr[m_position]);
	m_documentItemArr.erase(std::next(m_documentItemArr.begin(), m_position));
}

void CDeleteDocumentItemCommand::DoUnexecute()
{
	m_documentItemArr.insert(std::next(m_documentItemArr.begin(), m_position), *m_documentItem);
}
