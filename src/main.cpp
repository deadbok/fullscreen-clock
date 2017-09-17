#include "app.h"
#include <string>

int main(int argc, char **argv)
{
    int ret = 0;

    App *app = new App(std::string(".clockrc"));
    ret = app->run();

    return ret;
}
