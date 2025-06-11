#pragma once

#include "UIElement.h"
#include <string>
#include <functional>  // for std

class ButtonUI : public UIElement
{
public:
    ButtonUI(const std::string& label, ImVec2 position, ImVec2 size, std::function<void()> onClick)
        : m_label(label), m_position(position), m_size(size), m_onClick(onClick) {
    }

    void    draw() const override;

private:
    std::string     m_label;
    ImVec2          m_position;
    ImVec2          m_size;

    std::function<void()>   m_onClick;
};