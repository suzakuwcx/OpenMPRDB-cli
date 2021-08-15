#ifndef OPENMPRDB_CLI_SHELL_H
#define OPENMPRDB_CLI_SHELL_H

#include <memory>
#include <cstdio>
#include <exception>

//replace old_str to new_str in command
static void replace_all(std::string &command, std::string old_str, std::string new_str)
{
    unsigned long offset = 0;
    unsigned long i = command.find(old_str, offset);

    while (i != -1)
    {
        command.replace(i, old_str.size(), new_str);
        offset = i + new_str.size();
        i = command.find(old_str, offset);
    }
}

//execute a command in shell
static std::string execute(const char* command)
{
    std::unique_ptr<FILE, decltype(&pclose)> process(popen(command,"r"),pclose);
    std::string output;
    char buffer[256];

    if (!process)
        throw std::exception();
    while (fgets(buffer,256,process.get()) != nullptr)
        output += buffer;

    return output;
}

#endif //OPENMPRDB_CLI_SHELL_H
