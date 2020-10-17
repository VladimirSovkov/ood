#include "CPictureDraft.h"
#include "CPictureDraft.h"
#include <stdexcept>
#include <string>

CPictureDraft::CPictureDraft(const std::vector<std::shared_ptr<CShape>>& shapeArr)
	: m_shapeArr(shapeArr)
{
}

size_t CPictureDraft::GetShapeCount() const
{
	return m_shapeArr.size();
}

CShape& CPictureDraft::GetShape(size_t index) const
{
	if (index >= m_shapeArr.size())
	{
		throw std::out_of_range("index " + std::to_string(index) + " out of range " + std::to_string(m_shapeArr.size()));
	}
	return *m_shapeArr.at(index).get();
}
