#pragma once

#include "Command.h"

#include <vector>
#include <memory>


class Application
{
public:
    void registerCommand(std::unique_ptr<Command> command);
    int execute(int argc, char* argv[]);
private:
    // to store commands
    std::vector<std::unique_ptr<Command>> commands;
};

