#include "ECS/include/application.hpp"

int main(int argc, char *argv[])
{
    window::WindowSettings settings(1000, 800, "Test");
    application::MainApplication app(settings);

    auto res = app.loop();

    return res;
}