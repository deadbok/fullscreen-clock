#include "app.h"
#include <iostream>
#include <string>

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    std::string config_filename;
    int ret = 0;

    config_filename = "";
    if (argc == 2)
    {
        config_filename = argv[1];
    }
    else
    {
        struct passwd *pwd = getpwuid(getuid());
        if (pwd)
        {
            config_filename = pwd->pw_dir;
        }
        else
        {
            // try the $HOME environment variable
            config_filename = getenv("HOME");
        }

        if (config_filename == "")
        {
            std::cerr << "Could not get path to users home directory"
                      << std::endl;
            exit(1);
        }

        config_filename += "/.clockrc";
    }
    if (config_filename == "")
    {
        std::cerr << "No configuration file path" << std::endl;
        exit(1);
    }

    try
    {

        App *app = new App(std::string(config_filename));
        ret = app->run();
    }
    catch (const ConfigException &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    exit(ret);
}
