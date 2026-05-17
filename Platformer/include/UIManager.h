#pragma once

#include <vector>
#include <memory>
#include "UIElement.h"

class UIManager
{
public:
    ~UIManager();

    void    addElement(std::unique_ptr<UIElement> element);
    void    drawAll() const;

    ImFont*     fontSmall;
    ImFont*     fontMedium;
    ImFont*     fontLarge;

private:
    std::vector<std::unique_ptr<UIElement>>     m_elements;
};

/* EXAMPLE OF USE IN THE CODE
// Initialization
UIManager uiManager;
uiManager.addElement(std::make_unique<CircleUI>(
    LibMath::Vector2(200.0f, 300.0f), 30.0f, ImColor(255, 100, 50, 255)
));

// In renderFrame()
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

uiManager.drawAll(); // Draws all elements, extensible

ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
*/