#pragma once
#include "CPictureDraft.h"

class IDesigner
{
public:
	virtual ~IDesigner() = default;

	virtual CPictureDraft CreateDraft(std::istream& stream) = 0;
};