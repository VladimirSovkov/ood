#pragma once 
#include "CDesigner.h"
#include <sstream>

CDesigner::CDesigner(IShapeFactory& factory)
	: m_factory(factory)
{
}

CPictureDraft CDesigner::CreateDraft(std::istream& stream)
{
	std::vector<std::shared_ptr<CShape>> shapeArr;
	std::string line;

	while (getline(stream, line))
	{
		shapeArr.push_back(m_factory.CreateShape(line));
	}

	return CPictureDraft(shapeArr);
}
