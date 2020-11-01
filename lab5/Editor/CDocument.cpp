#include "CDocument.h"
#include "CommandFunc.h"
#include "CInsertDocumentItemCommand.h"
#include "CParagraph.h"
#include "CImage.h"
#include "CDeleteDocumentItemCommand.h"
#include <fstream>

const std::string IMAGES_DIR_NAME = "image";

std::shared_ptr<IParagraph> CDocument::InsertParagraph(const std::string& text, std::optional<size_t> position)
{
    CommandFunc func = std::bind(&CHistory::AddAndExecuteCommand, &m_history, std::placeholders::_1);
    std::shared_ptr<IParagraph> paragraphPtr = std::make_shared<CParagraph>(func, text);
    CDocumentItem documentItem(paragraphPtr);

    std::unique_ptr<ICommand> commandPtr = std::make_unique<CInsertDocumentItemCommand>(documentItem, m_documentItemArr, position);
    m_history.AddAndExecuteCommand(std::move(commandPtr));

    return paragraphPtr;
}

std::shared_ptr<IImage> CDocument::InsertImage(const Path& path, int width, int height, std::optional<size_t> position)
{
   CommandFunc func = std::bind(&CHistory::AddAndExecuteCommand, &m_history, std::placeholders::_1);
   std::shared_ptr<IImage> imagePtr = std::make_shared<CImage>(func, path, width, height);
   CDocumentItem documentItem(imagePtr);

   std::unique_ptr<ICommand> commandPtr = std::make_unique<CInsertDocumentItemCommand>(documentItem, m_documentItemArr, position);
   m_history.AddAndExecuteCommand(std::move(commandPtr));

    return imagePtr;
}

size_t CDocument::GetItemsCount() const
{
    return m_documentItemArr.size();
}

CConstDocumentItem CDocument::GetItem(size_t index) const
{
    return m_documentItemArr.at(index);
}

CDocumentItem CDocument::GetItem(size_t index)
{
    return m_documentItemArr.at(index);
}

void CDocument::DeleteItem(size_t index)
{
    m_history.AddAndExecuteCommand(std::make_unique<CDeleteDocumentItemCommand>(m_documentItemArr, index));
}

std::string CDocument::GetTitle() const
{
    return m_title;
}

void CDocument::SetTitle(const std::string& title)
{
    m_history.AddAndExecuteCommand(std::make_unique<CReplaceTextCommand>(m_title, title));
}

bool CDocument::CanUndo() const
{
    return m_history.CanUndo();
}

void CDocument::Undo()
{
    m_history.Undo();
}

bool CDocument::CanRedo() const
{
    return m_history.CanRedo();
}

void CDocument::Redo()
{
    m_history.Redo();
}

Path CreateImageDirectory(Path const& path)
{
    Path directory = path.parent_path() / IMAGES_DIR_NAME;
    if (!std::filesystem::is_directory(directory))
    {
        std::filesystem::create_directory(directory);
    }
    return directory;
}

std::string ConvertToHtmlStr(std::string str)
{
    std::string htmlStr;
    for (auto ch : str)
    {
        switch (ch)
        {
        case '<':
            htmlStr += "&lt;";
            break;
        case '>':
            htmlStr += "&gt;";
            break;
        case '&':
            htmlStr += "&amp;";
            break;
        case '"':
            htmlStr += "&quot;";
            break;
        case '\'':
            htmlStr += "&apos;";
            break;
        default:
            htmlStr += ch;
            break;
        }
    }

    return htmlStr;
}

std::string HeadHtml(std::string const& title)
{
    return "<head>\n\t<title>"
        + ConvertToHtmlStr(title) + "</title>\n</head>\n";
}

std::string ImageToHtml(Path const& src, int width, int height)
{
    return "<img src=\"" + ConvertToHtmlStr(src.string()) + "\" width=\"" +
        std::to_string(width) + "\" height=\"" + 
        std::to_string(height) + "\">\n";
}

std::string ParagraphToHtml(std::string const& text)
{
    return "<p>" + ConvertToHtmlStr(text) + "</p>\n";
}

void CDocument::Save(const Path& path) const
{
    std::ofstream html(path);
    Path directory = CreateImageDirectory(path);
    html << "<!DOCTYPE html>\n<html>\n" << HeadHtml(m_title) << "<body>\n";
    for (const CConstDocumentItem& documentItem : m_documentItemArr)
    {
        std::shared_ptr<const IImage> imagePtr = documentItem.GetImage();
        if (imagePtr)
        {
            Path path = imagePtr->GetPath();
            int width = imagePtr->GetWidth();
            int height = imagePtr->GetHeight();
            
            html << ImageToHtml(path, width, height);
            if (!std::filesystem::exists(directory / path.filename()))
            {
                std::filesystem::copy_file(path, directory / path.filename());
            }
        }
        else
        {
            html << ParagraphToHtml(documentItem.GetParagraph()->GetText());
        }
    }
    html << "</body>\n" << "</html>" << std::endl;
}