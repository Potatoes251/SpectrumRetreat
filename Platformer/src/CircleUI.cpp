#include "CircleUI.h"

CircleUI::CircleUI(const LibMath::Vector2& center, float radius, const ImColor& color)
    : m_center(center), m_radius(radius), m_color(color)
{
}

void    CircleUI::draw() const
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    if (draw_list)
    {
        draw_list -> AddCircleFilled(
            ImVec2(m_center.getX(), m_center.getY()),
            m_radius,
            m_color
        );
    }
}

void    CircleUI::setPosition(const LibMath::Vector2& position)
{
    m_center = position;
}

void    CircleUI::setRadius(float radius)
{
    m_radius = radius;
}

void    CircleUI::setColor(const ImColor& color)
{
    m_color = color;
}

ImColor     CircleUI::getColor()
{
    return m_color;
}