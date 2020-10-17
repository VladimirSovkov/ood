#pragma once
#include "CShape.h"
#include <memory>
#include <string>

class IShapeFactory
{
public:
	virtual ~IShapeFactory() = default;

	virtual std::shared_ptr<CShape> CreateShape(const std::string& description) = 0;
};