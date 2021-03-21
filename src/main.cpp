#include "Application.h"

#include <memory>

int main(int argc, const char** argv)
{
    auto application = std::make_unique<Application>("http://192.168.1.144", 33333);
    application->run();

    return 0;
}