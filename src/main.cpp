#include "render.h"

Renderer renderer;

int main(int, char **)
{
    if(renderer.Init("Calculator") != 0)
    {
        return -1;
    }
    while (renderer.IsRunning())
    {
        renderer.ProcessEvents();
        renderer.BeginFrame();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::SetNextWindowSize(renderer.GetWindowSize());
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("main", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::End();
        ImGui::PopStyleVar(1);

        renderer.EndFrame();
    }
    renderer.Shutdown();
    return 0;
}
