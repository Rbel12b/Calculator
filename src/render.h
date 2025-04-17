#pragma once
#ifndef _RENDER_H_
#define _RENDER_H_

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <functional>

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
            shutdown();
        }
    }

    int init(const char* windowTitle);
    void beginFrame();
    void endFrame();
    void shutdown();
    void processEvents();
    bool isRunning() const
    {
        return running;
    }
    void setEventCallback(std::function<void(SDL_Event&)> callback)
    {
        m_eventCallback = callback;
    }
    ImVec2 getWindowSize() const
    {
        return ImVec2(windowWidth, windowHeight);
    }
private:
    bool running = false;
    int windowHeight, windowWidth;
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::function<void(SDL_Event&)> m_eventCallback = nullptr;
};

#endif