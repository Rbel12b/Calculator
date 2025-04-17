#pragma once
#ifndef _RENDER_H_
#define _RENDER_H_

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer()
    {

    }
    ~Renderer()
    {
        if (running)
        {
            Shutdown();
        }
    }

    int Init(const char* windowTitle);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
    void ProcessEvents();
    bool IsRunning() const
    {
        return running;
    }
    void SetEventCallback(void (*callback)(SDL_Event &event))
    {
        m_eventCallback = callback;
    }
    ImVec2 GetWindowSize() const
    {
        return ImVec2(windowWidth, windowHeight);
    }
private:
    bool running = false;
    int windowHeight, windowWidth;
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void (*m_eventCallback)(SDL_Event &event) = nullptr;
};

#endif