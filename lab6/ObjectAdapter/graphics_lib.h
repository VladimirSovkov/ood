#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>

namespace graphics_lib
{
    // Холст для рисования
    class ICanvas
    {
    public:
        virtual void SetColor(uint32_t rgbColor) = 0;
        // Ставит "перо" в точку x, y
        virtual void MoveTo(int x, int y) = 0;
        // Рисует линию с текущей позиции, передвигая перо в точку x,y
        virtual void LineTo(int x, int y) = 0;
        virtual ~ICanvas() = default;
    };

    // Реализация холста для рисования
    class CCanvas : public ICanvas
    {
    public:
        void SetColor(uint32_t rgbColor) override
        {
            std::ostringstream ss;
            ss << std::setfill('0') << std::setw(8) << std::hex << rgbColor;
            std::cout << "SetColor (#" << ss.str() << ')' << std::endl;
            // TODO: вывести в output цвет в виде строки SetColor (#RRGGBB)
        }
        void MoveTo(int x, int y) override
        {
            std::cout << "MoveTo (" << x << ", " << y << ")" << std::endl;
        }
        void LineTo(int x, int y) override
        {
           std::cout << "LineTo (" << x << ", " << y << ")" << std::endl;
        }
    };
}
