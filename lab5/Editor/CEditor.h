#pragma once
#include "CMenu.h"
#include "IDocument.h"

class CEditor
{
public:
	CEditor(std::istream& input, std::ostream& output);

	void Run();

private:

	void InsertParagraph(std::istream& input);
	void InsertImage(std::istream& input);
	void SetTitle(std::istream& input);
	void ReplaceText(std::istream& input);
	void ResizeImage(std::istream& input);
	void DeleteItem(std::istream& input);
	void Save(std::istream& input);
	void Undo(std::istream&);
	void Redo(std::istream&);
	void List(std::istream&);
	
	void InitCommands();

	CMenu m_menu;
	std::unique_ptr<IDocument> m_documentPtr;
	std::ostream& m_output;
};

