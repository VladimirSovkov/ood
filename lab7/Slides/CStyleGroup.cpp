#include "CStyleGroup.h"

CStyleGroup::CStyleGroup(StyleEnumerator styleEnum)
    : m_styleEnum(styleEnum)
{
}

std::optional<bool> CStyleGroup::IsEnabled() const
{
    std::optional<bool> isEnabled;
    bool isInit = false;
    auto callback = [&](IStyle& style) {
        if (!(isEnabled.has_value() && isInit))
        {
            isEnabled = style.IsEnabled();
            isInit = true;
        }
        else if (isEnabled != style.IsEnabled())
        {
            isEnabled = std::nullopt;
        }
    };

    m_styleEnum(callback);
    return isEnabled;
}

void CStyleGroup::Enable(bool enable)
{
    auto callback = [&](IStyle& style) {
        style.Enable(enable);
    };
    m_styleEnum(callback);
}

std::optional<RGBAColor> CStyleGroup::GetColor() const
{
    std::optional<RGBAColor> color;
    bool isInit = false;
    auto callback = [&](IStyle& style) {
        if (!(color.has_value() && isInit))
        {
            color = style.GetColor();
            isInit = true;
        }
        else if (color != style.GetColor())
        {
            color = std::nullopt;
        }
    };

    m_styleEnum(callback);
    return color;
}

void CStyleGroup::SetColor(RGBAColor color)
{
    auto callback = [&](IStyle& style) {
        style.SetColor(color);
    };
    m_styleEnum(callback);
}
