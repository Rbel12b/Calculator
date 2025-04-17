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
        renderer.Render();
    }
    renderer.Shutdown();
    return 0;
}
