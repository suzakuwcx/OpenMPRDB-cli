#include "MPRDBC.h"
#include <cstring>
#include "iostream"
#include "value.h"
#include "GPGTool.h"
#include "HttpConnection.h"

struct Arguments
{
    bool showHelp = false;
    std::string opera = "";
    std::pair<bool,std::string> key = {false,""};
    std::pair<bool,std::string> player = {false,""};
    std::pair<bool,std::string> remote = {false,""};
    std::pair<bool,std::string> comment = {false,""};
    std::pair<bool,std::string> banList = {false,""};
    std::pair<bool,std::string> submit = {false,""};
};
//expand option like: -ls --> -l -s
void expandArgv(int &argc, char **&argv)
{
    //reserve
}

Arguments parseOption(int argc, char **argv)
{
    expandArgv(argc, argv);
    Arguments arguments;
    if (argc <= 1)
    {
        arguments.showHelp = true;
        return arguments;
    }

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        arguments.showHelp = true;
        return arguments;
    }
    else if (!strcmp(argv[1], "new") || !strcmp(argv[1], "delete") || !strcmp(argv[1], "import"))
    {
        arguments.opera = std::string(argv[1]);
    }
    else throw std::runtime_error("missing operation");

    for (int argN = 2; argN < argc; ++argN)
    {
        std::string option = {argv[argN]};

        if (option == "-h" || option == "--help")
        {
            arguments.showHelp = true;
            break;
        }
        else if (option == "--key")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("invalid use of option: " + option);
            arguments.key.first = true;
            arguments.key.second = argv[argN];
        }
        else if (option == "--player")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("invalid use of option: " + option);
            arguments.player.first = true;
            arguments.player.second = argv[argN];
        }
        else if (option == "--remote")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("invalid use of option: " + option);
            arguments.remote.first = true;
            arguments.remote.second = argv[argN];
        }
        else if (option == "--comment")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("invalid use of option: " + option);
            arguments.comment.first = true;
            arguments.comment.second = argv[argN];
        }
        else if (option == "--banList")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("invalid use of option: " + option);
            arguments.banList.first = true;
            arguments.banList.second = argv[argN];
        }
        else if (option == "--submit")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("invalid use of option: " + option);
            arguments.submit.first = true;
            arguments.submit.second = argv[argN];
        }
        else
            throw std::runtime_error("invalid use of option: " + option);
    }
    return arguments;
}

int main(int argc,char **argv)
{
    Arguments arguments;
    try
    {
        arguments = parseOption(argc,argv);
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    MPRDBC mprdbc;
    if (arguments.showHelp)
    {
        mprdbc.printHelp();
        return 0;
    }
    if (arguments.opera == "new")
    {
        Value value;

        if (!arguments.key.first)
            throw std::exception();
        GPGTool tool(arguments.key.second.c_str());
        value["message"] = tool.sign("server_name: Some server1");
        value["public_key"] = tool.getKey();
        HttpConnection connection;
        connection.setMethod(kPut);
        connection.setUrl("https://test.openmprdb.org/v1/server/register");
        connection.setMessage(value.transform().c_str(),kJson);

        std::string reply = connection.connect();
        return 0;
    }
    return 0;
}
