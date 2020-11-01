#include "CResizeImageCommand.h"

CResizeImageCommand::CResizeImageCommand(int& width, int& height, int newWidth, int newHeight)
	:m_widthHeight(width, height)
	, m_nextWidthHeight(newWidth, newHeight)
{
}

void CResizeImageCommand::DoExecute()
{
	SwapPairs();
}

void CResizeImageCommand::DoUnexecute()
{
	SwapPairs();
}

void CResizeImageCommand::SwapPairs()
{
	std::swap(m_widthHeight.first, m_nextWidthHeight.first);
	std::swap(m_widthHeight.second, m_nextWidthHeight.second);
}
