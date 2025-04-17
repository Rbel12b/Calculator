#pragma once
#ifndef _RENDER_H_
#define _RENDER_H_

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
/*
 * -----------------------------------------------------------------------------
 *  File:           render.h
 *  Project:        Calculator
 *  Author:         Rbel12b (https::/github.com/rbel12b)
 *  Description:    class for the imgui backend (SDL2)
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