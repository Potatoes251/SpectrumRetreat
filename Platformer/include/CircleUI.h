#pragma once


#include "UIElement.h"
#include "LibMath/Vector/Vector2.h"

class CircleUI : public UIElement
{
public:
    CircleUI(const LibMath::Vector2& center, float radius, const ImColor& color);

    void    draw() const override;

    void    setPosition(const LibMath::Vector2& position);
    void    setRadius(float radius);
    void    setColor(const ImColor& color);
    ImColor     getColor();

private:
    LibMath::Vector2    m_center;
    float       m_radius;
    ImColor     m_color;
};