#include "WakeWord/Watchdog.h"
#include <iostream>

void Fun(void *path)
{
    std::cout << "Hi I am 小白" << std::endl;
}

int main(int argc, char const *argv[])
{
    std::string resource("common.res");
    std::string model("./models/WakeWord/model.pmdl");
    std::string sensitivity("0.5");
    bool is_exit = false;
    Watchdog::working(resource, model, sensitivity, &is_exit, Fun, nullptr);
    return 0;
}
