#pragma once
#ifndef _APP_H_
#define _APP_H_

#include "render.h"

class App
{
public:
    App(){}
    ~App()
    {
        if (running)
        {
            shutdown();
        }
    }

    int init(const char* windowTitle);
    int run();
    void shutdown();
private:
    void processEvents(SDL_Event &event);
    void render();
private:
    bool running = false;
    Renderer renderer;
};
#endif