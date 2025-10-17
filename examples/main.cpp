#include "Core/Application.h"

int main()
{
    nre::ApplicationConfig config;
    config.title = "NanoRender Example";
    config.width = 800;
    config.height = 600;

    nre::Application app(config);
    app.run();
    return 0;
}
