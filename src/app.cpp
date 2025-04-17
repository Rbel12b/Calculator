#include "app.h"
#include <functional>

int App::init(const char *windowTitle)
{
    renderer.setEventCallback([this](SDL_Event &event) { processEvents(event); });
    if(renderer.init(windowTitle))
    {
        return -1;
    }
    running = true;
    return 0;
}

int App::run()
{
    while (running && renderer.isRunning())
    {
        renderer.processEvents();
        renderer.beginFrame();
        render();
        renderer.endFrame();
    }
    shutdown();
    return 0;
}

void App::shutdown()
{
    renderer.shutdown();
    running = false;
}

void App::processEvents(SDL_Event &event)
{
    if (event.type == SDL_QUIT)
    {
        running = false;
    }
}

void App::render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(renderer.getWindowSize());

    if (ImGui::Begin("mainWindow", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("Hello World!");
    }
    ImGui::End();

    ImGui::PopStyleVar(1);
}
