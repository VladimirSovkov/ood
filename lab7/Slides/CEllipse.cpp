#include "CEllipse.h"

CEllipse::CEllipse(Point center, double horizontalRadius, double verticalRadius)
    :m_center(center)
    , m_horizontalRadius(horizontalRadius)
    , m_verticalRadius(verticalRadius)
{
}

Point CEllipse::GetCenter() const
{
    return m_center;
}

double CEllipse::GetHorizontalRadius() const
{
    return m_horizontalRadius;
}

double CEllipse::GetVerticalRadius() const
{
    return m_verticalRadius;
}

std::optional<RectD> CEllipse::GetFrame() const
{
    double leftTopX = m_center.x - m_horizontalRadius;
    double leftTopY = m_center.y - m_verticalRadius;
    return RectD{ {leftTopX, leftTopY}, m_horizontalRadius * 2., m_verticalRadius * 2. };
}

void CEllipse::SetFrame(const RectD& rect)
{
    m_horizontalRadius = rect.width / 2.;
    m_verticalRadius = rect.height / 2.;
    m_center.x = rect.leftTop.x + (rect.width / 2.);
    m_center.y = rect.leftTop.y + (rect.height / 2.);
}

void CEllipse::DrawLine(ICanvas& canvas) const
{
    canvas.DrawEllipse(m_center, m_verticalRadius, m_horizontalRadius);
}

void CEllipse::DrawFill(ICanvas& canvas) const
{
    canvas.FilEllipse(m_center, m_verticalRadius, m_horizontalRadius);
}
