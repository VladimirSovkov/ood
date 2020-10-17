#pragma once
#include <vector>
#include <memory>
#include "CShape.h"

class CPictureDraft
{
public:
	CPictureDraft(const std::vector<std::shared_ptr<CShape>>& shapeArr);

	size_t GetShapeCount() const;
	CShape& GetShape(size_t index) const;

private:
	std::vector<std::shared_ptr<CShape>> m_shapeArr;
};
