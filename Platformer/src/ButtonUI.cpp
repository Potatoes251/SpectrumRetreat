#include "ButtonUI.h"
#include <imgui.h>

void    ButtonUI::draw() const
{
    ImGui::SetNextWindowPos(m_position);
    ImGui::SetNextWindowSize(m_size);
    ImGui::Begin(("##" + m_label).c_str(), nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground);

    if (ImGui::Button(m_label.c_str(), m_size))
        m_onClick();

    ImGui::End();
}