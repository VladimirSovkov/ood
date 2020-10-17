#pragma once
#include "IShapeFactory.h"
#include "CTriangle.h"
#include "CRectangle.h"
#include "CRegularPolygon.h"
#include "CEllipse.h"
#include <map>
#include <functional>

class CShapeFactory : public IShapeFactory
{
public:
	CShapeFactory();
	virtual ~CShapeFactory() = default;

	std::shared_ptr<CShape> CreateShape(const std::string& description) override;

private:
	std::map<std::string, std::function<std::shared_ptr<CShape>(std::istream&)>> m_shapeCreationMap;
};
