#include "CPainter.h"

void CPainter::DrawPicture(const CPictureDraft& draft, ICanvas& canvas) const
{
	size_t	shapeCount = draft.GetShapeCount();
	for (size_t i = 0; i < shapeCount; i++)
	{
		draft.GetShape(i).Draw(canvas);
	}
}
