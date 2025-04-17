#include "app.h"

App app;

int main(int, char **)
{
    if(app.init("Calculator"))
    {
        return -1;
    }
    return app.run();
}
