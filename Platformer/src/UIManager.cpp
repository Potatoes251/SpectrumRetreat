#include "UIManager.h"

void UIManager::addElement(std::unique_ptr<UIElement> element)
{
    m_elements.push_back(std::move(element));
}

void UIManager::drawAll() const
{
    for (const auto& element : m_elements)
    {
        if (element->isActive)
            element->draw();
    }
}