#include "UIManager.h"

UIManager::~UIManager()
{
    m_elements.clear();

    fontSmall = nullptr;
    fontMedium = nullptr;
    fontLarge = nullptr;


}

void UIManager::addElement(std::unique_ptr<UIElement> element)
{
    m_elements.push_back(std::move(element));
}

void UIManager::drawAll() const
{
    for (const auto& element : m_elements)
    {
        if (element -> m_isActive)
            element -> draw();
    }
}