/*
 * -----------------------------------------------------------------------------
 *  File:           app.cpp
 *  Project:        Calculator
 *  Author:         Rbel12b (https::/github.com/rbel12b)
 *  Description:    main app implementation
 * -----------------------------------------------------------------------------
 *  License:        MIT License
 *
 *  Copyright (c) 2025 Rbel12b
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 * -----------------------------------------------------------------------------
 */
#include "app.h"
#include <functional>
#include "ImGuiCalculatorInput.h"
#include <chrono>
#include <thread>

const char* calcInputName = "calcInput";
ImGuiID calcInpuID;

int App::init(const char *windowTitle)
{
    renderer.setEventCallback([this](SDL_Event &event) { processEvents(event); });
    if(renderer.init(windowTitle))
    {
        return -1;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGuiCalculatorInput::init();
    running = true;
    return 0;
}

int App::run()
{
    while (running && renderer.isRunning())
    {
        auto start = std::chrono::high_resolution_clock::now();
        renderer.processEvents();
        renderer.beginFrame();
        render();
        renderer.endFrame();
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (elapsed < frame_time_ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frame_time_ms - elapsed));
        }
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
        // Fill the entire window with a dock space
        ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
        ImGui::DockSpace(dockspace_id);
    }
    ImGui::End();
    
    calcInpuID = ImGui::GetID(calcInputName);

    ImGuiCalculatorInput::render(calcInputName, calcInpuID);

    CalcInputData& input = ImGuiCalculatorInput::getInput(calcInpuID);

    if (input.enterPressed)
    {
        printf("Entered: %s\n", input.text.c_str());
        input.text.clear();
        input.enterPressed = false;
    }

    ImGui::PopStyleVar(1);
}
