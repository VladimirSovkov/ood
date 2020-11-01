#include "CResizeImageCommand.h"
#include <stdexcept>

CResizeImageCommand::CResizeImageCommand(int& width, int& height, int newWidth, int newHeight)
	:m_widthHeight(width, height)
{
	if (newWidth < 1 || newHeight < 1 || newWidth > 10000 || newHeight > 10000)
	{
		throw std::invalid_argument("image size cannot be negative");
	}
	m_nextWidthHeight = { newWidth, newHeight };
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
