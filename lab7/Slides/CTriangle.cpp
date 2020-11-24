#include "CTriangle.h"
#include <algorithm>

CTriangle::CTriangle(Point vertex1, Point vertex2, Point vertex3)
    :m_vertex1(vertex1)
    , m_vertex2(vertex2)
    , m_vertex3(vertex3)
{
}

Point CTriangle::GetVertex1() const
{
    return m_vertex1;
}

Point CTriangle::GetVertex2() const
{
    return m_vertex2;
}

Point CTriangle::GetVertex3() const
{
    return m_vertex3;
}

std::optional<RectD> CTriangle::GetFrame() const
{
    auto minMaxX = std::minmax({ m_vertex1.x, m_vertex2.x, m_vertex3.x });
    auto minMaxY = std::minmax({ m_vertex1.y, m_vertex2.y, m_vertex3.y });

    return RectD({{ minMaxX.first , minMaxY.first }, minMaxX.second - minMaxX.first, minMaxY.second - minMaxY.first});
}

Point CalculateNewPosition(const Point& point, const RectD& newRect, const RectD& oldRect)
{
    double widthRatio = oldRect.width ? newRect.width / oldRect.width : 0.0;
    double heightRatio = oldRect.height ? newRect.height / oldRect.height : 0.0;

    double vertexX = widthRatio * (point.x - oldRect.leftTop.x) + newRect.leftTop.x;
    double vertexY = heightRatio * (point.y - oldRect.leftTop.y) + newRect.leftTop.y;
    return { vertexX, vertexY };
}

void CTriangle::SetFrame(const RectD& rect)
{
    RectD oldRect = GetFrame().value();
    m_vertex1 = CalculateNewPosition(m_vertex1, rect, oldRect);
    m_vertex2 = CalculateNewPosition(m_vertex2, rect, oldRect);
    m_vertex3 = CalculateNewPosition(m_vertex3, rect, oldRect);
}

void CTriangle::DrawLine(ICanvas& canvas) const
{
    canvas.DrawLine(m_vertex1, m_vertex2);
    canvas.DrawLine(m_vertex2, m_vertex3);
    canvas.DrawLine(m_vertex3, m_vertex1);
}

void CTriangle::DrawFill(ICanvas& canvas) const
{
    std::vector<Point> pointArr{
        m_vertex1,
        m_vertex2,
        m_vertex3,
    };
    canvas.FillPoligon(pointArr);
}