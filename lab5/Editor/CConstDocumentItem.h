#pragma once
#include <memory>
#include "IImage.h"
#include "IParagraph.h"

class CConstDocumentItem
{
public:
	CConstDocumentItem(const std::shared_ptr<IImage>& imagePtr);
	CConstDocumentItem(const std::shared_ptr<IParagraph>& paragraphPtr);

	// ���������� ��������� �� ����������� �����������, ���� nullptr, ���� ������� �� �������� ������������
	std::shared_ptr<const IImage> GetImage()const;
	// ���������� ��������� �� ����������� ��������, ���� nullptr, ���� ������� �� �������� ����������
	std::shared_ptr<const IParagraph> GetParagraph()const;
	virtual ~CConstDocumentItem() = default;

private:
	std::shared_ptr<const IImage> m_imagePtr = nullptr;
	std::shared_ptr<const IParagraph> m_paragraphPtr = nullptr;
};


