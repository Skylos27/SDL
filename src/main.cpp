#include "ECS/include/application.hpp"

int main(int argc, char *argv[])
{
    if (argc != 4)
        throw std::runtime_error("Need three arguments - "
                             "number of sheep, number of wolves, "
                             "simulation time\n");

    uint16_t sheep = std::strtoul(argv[1], nullptr, 0);
    uint16_t wolves = std::strtoul(argv[2], nullptr, 0);
    uint16_t time = std::strtoul(argv[3], nullptr, 0);
    window::WindowSettings settings(1800, 900, "Projet SDL2", sheep, wolves, time);
    application::MainApplication app(settings);

    auto res = app.loop();

    return res;
}