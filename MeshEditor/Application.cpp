#include "Application.h"

#include <iostream>
#include <map>

void Application::registerCommand(std::unique_ptr<Command> command)
{
    this->commands.push_back(std::move(command));
}

int Application::execute(int argc, char* argv[])
{
    if (argc == 1)
    {
        return 1;
    }

    for (size_t i = 0; i < commands.size(); i++)
    {
        if (argv[1] == commands[i]->getName())
        {
            std::map <std::string, std::string> args;

            for (size_t j = 2; j < argc; j++)
            {
                if (!strcmp(argv[j], "L") || !strcmp(argv[j], "filepath") ||
                    !strcmp(argv[j], "R") || !strcmp(argv[j], "filename") ||
                    !strcmp(argv[j], "output1") || !strcmp(argv[j], "output2") ||
                    !strcmp(argv[j], "input"))
                {
                    std::string temp = argv[j + 2];

                    if (temp.back() == ',')
                        temp.pop_back();

                    args.emplace(argv[j], temp);
                }

                if (!strcmp(argv[j], "origin") || !strcmp(argv[j], "direction"))
                {
                    std::string origin = argv[j + 2];

                    int k = 3;
                    while (origin.back() != ')' && origin.at(origin.length() - 2) != ')')
                    {
                        origin += argv[j + k];
                        ++k;
                    }

                    if (origin.back() == ',')
                        origin.pop_back();

                    args.emplace(argv[j], origin);
                }

                if (!strcmp(argv[j], "slices") || !strcmp(argv[j], "rings"))
                {
                    args.emplace(argv[j], argv[j + 2]);
                }
            }

            commands[i]->execute(args);
            return 0;
        }
    }

    return 1;
}
