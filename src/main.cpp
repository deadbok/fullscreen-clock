#include "app.h"
#include <string>

int main(int argc, char **argv)
{
    std::string config_filename;
    int ret = 0;

    config_filename = ".clockrc";
    if (argc == 2)
    {
        config_filename = argv[1];
    }

    App *app = new App(std::string(config_filename));
    ret = app->run();

    return ret;
}
