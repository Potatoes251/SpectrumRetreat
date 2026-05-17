#pragma once


#include <imgui.h>

class UIElement
{
public:
    virtual     ~UIElement() = default;

    virtual void    draw() const = 0;

    bool    m_isActive = true;
};