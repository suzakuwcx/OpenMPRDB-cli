#ifndef OPENMPRDB_CLI_SHELL_HPP
#define OPENMPRDB_CLI_SHELL_HPP

#include <memory>
#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <wait.h>

#define READ 0
#define WRITE 1

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
    if (strchr(command,'\n') != nullptr)
        throw std::runtime_error("contain newline,interrupt");

    FILE* process = popen(command,"r");
    std::string output;
    char buffer[256];

    if (!process)
        throw std::runtime_error("create process fail");
    while (fgets(buffer,sizeof (buffer),process) != nullptr)
        output += buffer;

    if (pclose(process) == EXIT_SUCCESS)
        return output;
    else
        throw std::runtime_error("execute fail");
}

//execute a command in shell
static std::string execute(const char* command,const std::vector<std::string>& params,const std::string &input = "")
{
    bool hasInput = false;
    if (!input.empty())
        hasInput = true;

    //append NULL and command： {"--help"} ==> {<command>,--help,NULL}
    unsigned long n = params.size();
    char * argv[n + 2];

    argv[0] = const_cast<char *> (command);
    for (int i = 0; i < n; ++i)
    {
        argv[i + 1] = const_cast<char*>(params[i].c_str());
    }
    argv[n + 1] = nullptr;

    std::string reply;
    char buff[64] = {};

    int pipe_out[2];
    int pipe_in[2];
    pipe(pipe_out);
    pipe(pipe_in);

    pid_t pid = fork();

    if (pid < 0)
        throw std::runtime_error("fork fail");

    if (pid == 0)
    {
        //child process
        close(pipe_out[READ]);
        dup2(pipe_out[WRITE],STDOUT_FILENO);

        close(pipe_in[WRITE]);
        if (hasInput)
            dup2(pipe_in[READ],STDIN_FILENO);
        else
            close(pipe_in[READ]);

        execvp(command, argv);
        throw std::runtime_error("execute fail");
    }
    else
    {
        //parent process

        //writing input to child process
        close(pipe_in[READ]);
        if (hasInput)
            write(pipe_in[WRITE],input.c_str(),input.size());
        close(pipe_in[WRITE]);

        close(pipe_out[WRITE]);
        ssize_t len = read(pipe_out[READ],buff,sizeof (buff) - 1);
        while (len > 0)
        {
            buff[len] = '\0';
            reply += buff;
            len = read(pipe_out[READ],buff,sizeof (buff) - 1);
        }

        int status = 0;
        waitpid(pid, &status,0);

        if (status != 0)
            throw std::runtime_error("execute fail");
    }
    return reply;
}

#endif //OPENMPRDB_CLI_SHELL_HPP
