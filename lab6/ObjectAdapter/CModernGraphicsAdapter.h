#pragma once
#include "graphics_lib.h"
#include "modern_graphics_lib.h"

class CModernGraphicsAdapter : public graphics_lib::ICanvas
{
public: 
    CModernGraphicsAdapter(modern_graphics_lib::CModernGraphicsRenderer& renderer);

    void SetColor(uint32_t rgbColor) override;
    void MoveTo(int x, int y) override;
    void LineTo(int x, int y) override;

private:
    modern_graphics_lib::CModernGraphicsRenderer& m_renderer;
    modern_graphics_lib::CPoint m_point;
    modern_graphics_lib::CRGBAColor m_color;
};