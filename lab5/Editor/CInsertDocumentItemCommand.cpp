#include "CInsertDocumentItemCommand.h"

CInsertDocumentItemCommand::CInsertDocumentItemCommand(
	CDocumentItem documentItem, std::vector<CDocumentItem>& documentItemArr, std::optional<size_t> position)
	: m_documentItem(documentItem)
	, m_documentItemArr(documentItemArr)
	, m_position(position)
{
}

void CInsertDocumentItemCommand::DoExecute()
{
	if (m_position.has_value())
	{
		if (m_position >= m_documentItemArr.size())
		{
			throw std::out_of_range("Document item position is out of range");
		}
		m_documentItemArr.insert(std::next(m_documentItemArr.begin(), m_position.value()), m_documentItem);
	}
	else
	{
		m_documentItemArr.push_back(m_documentItem);
	}
}

void CInsertDocumentItemCommand::DoUnexecute()
{
	if (m_position.has_value())
	{
		m_documentItemArr.erase(std::next(m_documentItemArr.begin(), m_position.value()));
	}
	else
	{
		m_documentItemArr.pop_back();
	}
}
