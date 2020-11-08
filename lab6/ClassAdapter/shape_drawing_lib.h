#pragma once
#include <iostream>
#include "graphics_lib.h"

namespace shape_drawing_lib
{
	struct Point
	{
		int x;
		int y;
	};

	// »нтерфейс объектов, которые могут быть нарисованы на холсте из graphics_lib
	class ICanvasDrawable
	{
	public:
		virtual void Draw(graphics_lib::ICanvas& canvas)const = 0;
		virtual ~ICanvasDrawable() = default;
	};

	class CTriangle : public ICanvasDrawable
	{
	public:
		CTriangle(const Point& p1, const Point& p2, const Point& p3, uint32_t color)
			: m_p1(p1)
			, m_p2(p2)
			, m_p3(p3)
			, m_color(color)
		{
		}

		void Draw(graphics_lib::ICanvas& canvas)const override
		{
			canvas.SetColor(m_color);
			canvas.MoveTo(m_p1.x, m_p1.y);
			canvas.LineTo(m_p2.x, m_p2.y);
			canvas.LineTo(m_p3.x, m_p3.y);
			canvas.MoveTo(m_p2.x, m_p2.y);
			canvas.LineTo(m_p3.x, m_p3.y);
		}
	private:
		Point m_p1;
		Point m_p2;
		Point m_p3;
		uint32_t m_color;
	};

	class CRectangle : public ICanvasDrawable
	{
	public:
		CRectangle(const Point& leftTop, int width, int height, uint32_t m_color)
			: m_leftTop(leftTop)
			, m_width(width)
			, m_height(height)
			, m_color(m_color)
		{
		}

		void Draw(graphics_lib::ICanvas& canvas)const override
		{
			canvas.SetColor(m_color);
			canvas.MoveTo(m_leftTop.x, m_leftTop.y);
			canvas.LineTo(m_leftTop.x + m_width, m_leftTop.y);
			canvas.LineTo(m_leftTop.x, m_leftTop.y - m_height);
			Point bottom_right;
			bottom_right.x = m_leftTop.x + m_width;
			bottom_right.y = m_leftTop.y - m_height;
			canvas.MoveTo(bottom_right.x, bottom_right.y);
			canvas.LineTo(bottom_right.x - m_width, bottom_right.y);
			canvas.LineTo(bottom_right.x, bottom_right.y + m_height);
		}
	private:
		Point m_leftTop;
		int m_width;
		int m_height;
		uint32_t m_color;
	};

	// ’удожник, способный рисовать ICanvasDrawable-объекты на ICanvas
	class CCanvasPainter
	{
	public:
		CCanvasPainter(graphics_lib::ICanvas& canvas)
			:m_canvas(canvas)
		{}
		void Draw(const ICanvasDrawable& drawable)
		{
			drawable.Draw(m_canvas);
		}
	private:
		graphics_lib::ICanvas& m_canvas;
	};
}