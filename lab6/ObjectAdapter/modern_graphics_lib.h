 #pragma once
#include <sstream>

namespace modern_graphics_lib
{
	class CPoint
	{
	public:
		CPoint(int x, int y) 
			:x(x), y(y) 
		{}

		int x;
		int y;
	};

	class CRGBAColor
	{
	public:
		CRGBAColor(float r, float g, float b, float a) :r(r), g(g), b(b), a(a) {}
		float r, g, b, a;
	};

	// Класс для современного рисования графики
	class CModernGraphicsRenderer
	{
	public:
		CModernGraphicsRenderer(std::ostream& strm) : m_out(strm)
		{
		}

		~CModernGraphicsRenderer()
		{
			if (m_drawing) // Завершаем рисование, если оно было начато
			{
				EndDraw();
			}
		}

		// Этот метод должен быть вызван в начале рисования
		void BeginDraw()
		{
			if (m_drawing)
			{
				throw std::logic_error("Drawing has already begun");
			}
			m_out << "<draw>" << std::endl;
			m_drawing = true;
		}

		// Выполняет рисование линии
		void DrawLine(const CPoint& start, const CPoint& end, const CRGBAColor& color)
		{
			if (!m_drawing)
			{
				throw std::logic_error("DrawLine is allowed between BeginDraw()/EndDraw() only");
			}
			m_out << "  <line fromX=\"" << start.x
				<< "\" fromY=\"" << start.y
				<< "\" toX=\"" << end.x
				<< "\" toY=\"" << end.y << "\">\n"
				<< "    <color r=\"" << color.r
				<< "\" g=\"" << color.g
				<< "\" b=\"" << color.b
				<< "\" a=\"" << color.a << "\" />\n"
				<< "  </line>\n";
			// Можно вызывать только между BeginDraw() и EndDraw()
		}

		// Этот метод должен быть вызван в конце рисования
		void EndDraw()
		{
			if (!m_drawing)
			{
				throw std::logic_error("Drawing has not been started");
			}
			m_out << "</draw>" << std::endl;
			m_drawing = false;
		}
	private:
		std::ostream& m_out;
		bool m_drawing = false;
	};
}