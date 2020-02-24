#include "Application.h"

#include "Cube.h"
#include "Sphere.h"
#include "Split.h"

int main(int argc, char* argv[])
{
    Application app;

    app.registerCommand(std::make_unique<Cube>());
    app.registerCommand(std::make_unique<Sphere>());
    app.registerCommand(std::make_unique<Split>());

    return app.execute(argc, argv);
}