#pragma once
#include "CAbstractCommand.h"
#include <utility>

class CResizeImageCommand : public CAbstractCommand
{
public:
	CResizeImageCommand(int& widthContainer, int& heightContainer, int newWidth, int newHeight);

protected:
	virtual void DoExecute() override;
	virtual void DoUnexecute() override;

private:
	void SwapPairs();

	const std::pair<int&, int&> m_widthHeight;
	std::pair<int, int> m_nextWidthHeight;
};

