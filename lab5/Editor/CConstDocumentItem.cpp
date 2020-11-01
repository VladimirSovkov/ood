#include "CConstDocumentItem.h"

CConstDocumentItem::CConstDocumentItem(const std::shared_ptr<IImage>& imagePtr)
    :m_imagePtr(imagePtr)
{
}

CConstDocumentItem::CConstDocumentItem(const std::shared_ptr<IParagraph>& paragraphPtr)
    : m_paragraphPtr(paragraphPtr)
{
}

std::shared_ptr<const IImage> CConstDocumentItem::GetImage() const
{
    return m_imagePtr;
}

std::shared_ptr<const IParagraph> CConstDocumentItem::GetParagraph() const
{
    return m_paragraphPtr;
}
