#include "CEditor.h"
#include "CDocument.h"

CEditor::CEditor(std::istream& input, std::ostream& output)
	:m_documentPtr(std::make_unique<CDocument>())
	, m_menu(input, output)
	, m_output(output)
{
	InitCommands();
}

std::optional<size_t> ParsePosition(std::istream& input)
{
	std::string positionStr;
	input >> positionStr;
	std::optional<size_t> position = std::nullopt;
	try
	{
		if (positionStr != "end")
		{
			position = std::stoi(positionStr);
		}
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Invalid position argument: " + positionStr );
	}
	
	return position;
}

void CEditor::Run()
{
	m_menu.Run();
}

void CEditor::InsertParagraph(std::istream& input)
{
	try
	{
		std::optional<size_t> positionOpt = ParsePosition(input);
		std::string text;
		std::getline(input >> std::ws, text);

		m_documentPtr->InsertParagraph(text, positionOpt);
	}
	catch (const std::exception& exc)
	{
		m_output << exc.what() << std::endl;
	}
}

void CEditor::InsertImage(std::istream& input)
{
	try
	{
		std::optional<size_t> positionOpt = ParsePosition(input);
		int width;
		int height;
		std::string path;
		if (!((input >> width) && (input >> height)))
		{
			m_output << "invalid arguments" << std::endl;
			return;
		}
		getline(input >> std::ws, path);
		m_documentPtr->InsertImage(path, width, height, positionOpt);
	}
	catch (const std::exception& exc)
	{
		std::string answer = exc.what();
		m_output << answer << std::endl;
	}
}

void CEditor::SetTitle(std::istream& input)
{
	std::string title;
	std::getline(input >> std::ws, title);
	m_documentPtr->SetTitle(title);
}

void CEditor::ReplaceText(std::istream& input)
{
	try
	{
		std::optional<size_t> position = ParsePosition(input);
		if (!position)
		{
			m_output << "Invalid argument position " << std::endl;
			return;
		}
		std::shared_ptr<IParagraph> paragraphPtr = m_documentPtr->GetItem(position.value()).GetParagraph();
		if (!paragraphPtr)
		{
			m_output << "The document element at position " +
				std::to_string(position.value()) + " is not a paragraph" << std::endl;
			return;
		}
		std::string text;
		std::getline(input >> std::ws, text);
		paragraphPtr->SetText(text);
	}
	catch (const std::exception& exc)
	{
		m_output << exc.what() << std::endl;
	}

}

void CEditor::ResizeImage(std::istream& input)
{
	try
	{
		std::optional<size_t> position = ParsePosition(input);
		if (!position)
		{
			m_output << "Invalid argument position " << std::endl;
			return;
		}
		int width;
		int height;
		input >> width >> height;
		std::shared_ptr<IImage> imagePtr = m_documentPtr->GetItem(position.value()).GetImage();
		if (!imagePtr)
		{
			m_output << "The document element at position " +
				std::to_string(position.value()) + " is not a image" << std::endl;
			return;

		}
		imagePtr->Resize(width, height);
	}
	catch (const std::exception& exc)
	{
		m_output << exc.what() << std::endl;
	}
}

void CEditor::DeleteItem(std::istream& input)
{
	try
	{
		std::optional<size_t> position = ParsePosition(input);
		if (!position)
		{
			m_output << "Invalid argument position " << std::endl;
			return;
		}
		m_documentPtr->DeleteItem(position.value());
	}
	catch (const std::exception& exc)
	{
		m_output << exc.what() << std::endl;
	}

}

void CEditor::Save(std::istream& input)
{
	std::string path;
	input >> path;

	if (!path.size())
	{
		m_output << "Empty save directory given" << std::endl;
		return;
	}
	m_documentPtr->Save(path);
}

void CEditor::Undo(std::istream&)
{
	if (m_documentPtr->CanUndo())
	{
		m_documentPtr->Undo();
	}
	else
	{
		m_output << "Can't undo" << std::endl;
	}

}

void CEditor::Redo(std::istream&)
{
	if (m_documentPtr->CanRedo())
	{
		m_documentPtr->Redo();
	}
	else
	{
		m_output << "Can't redo" << std::endl;
	}
}

void CEditor::List(std::istream&)
{
	m_output << "Title: " << m_documentPtr->GetTitle() << std::endl;
	for (size_t i = 0; i < m_documentPtr->GetItemsCount(); i++)
	{
		CDocumentItem item = m_documentPtr->GetItem(i);
		std::shared_ptr<IImage> imagePtr = item.GetImage();
		if (imagePtr)
		{
			m_output << i << ". Image: " 
				<< imagePtr->GetWidth() << ' ' 
				<< imagePtr->GetHeight() << ' ' 
				<< imagePtr->GetPath() << std::endl;
		}
		else
		{
			m_output << i << ". Paragraph: "
				<< item.GetParagraph()->GetText() << std::endl;
		}
	}
}

void CEditor::InitCommands()
{
	m_menu.AddItem("InsertParagraph", "InsertParagraph <position>|end <paragraph text>", std::bind(&CEditor::InsertParagraph, this, std::placeholders::_1));
	m_menu.AddItem("InsertImage", "InsertImage <position>|end <width> <height> <image file path>", std::bind(&CEditor::InsertImage, this, std::placeholders::_1));
	m_menu.AddItem("SetTitle", "SetTitle <document title>", std::bind(&CEditor::SetTitle, this, std::placeholders::_1));
	m_menu.AddItem("List", "list", std::bind(&CEditor::List, this, std::placeholders::_1));
	m_menu.AddItem("ReplaceText", "ReplaceText <position> <paragraph text>", std::bind(&CEditor::ReplaceText, this, std::placeholders::_1));
	m_menu.AddItem("ResizeImage", "ResizeImage <position> <width> <height>", std::bind(&CEditor::ResizeImage, this, std::placeholders::_1));
	m_menu.AddItem("DeleteItem", "DeleteItem <position>", std::bind(&CEditor::DeleteItem, this, std::placeholders::_1));
	m_menu.AddItem("Help", "Help", [this](std::istream&) {m_menu.ShowInstructions(); });
	m_menu.AddItem("Undo", "Undo", std::bind(&CEditor::Undo, this, std::placeholders::_1));
	m_menu.AddItem("Redo", "Redo", std::bind(&CEditor::Redo, this, std::placeholders::_1));
	m_menu.AddItem("Save", "Save <path>", std::bind(&CEditor::Save, this, std::placeholders::_1));
	m_menu.AddItem("Exit", "Exit", [this](std::istream&) {m_menu.Exit(); });
}