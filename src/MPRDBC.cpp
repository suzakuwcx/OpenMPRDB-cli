#include <iostream>
#include "MPRDBC.h"
#include <cstring>
#include "value.h"
#include "GPGTool.h"
#include "HttpConnection.h"
#include <sys/stat.h>

void MPRDBC::printHelp()
{
    std::cout << "Usage: prd-client [COMMAND] [OPTIONS] \n"
    << "Available commands: \n"
    << "  config                  Generate default config\n"
    << "  register \n"
    << "  delete \n"
    << "  import \n"
    << "  commit \n"
    << "\n"
    << "Option: \n"
    << "  -h, --help              Display this help and exit \n"
    << "  --key        [value]                               \n"
    << "  --player     [value]                               \n"
    << "  --remote     [value]                               \n"
    << "  --comment    [value]                               \n"
    << "  --banList    [value]                               \n"
    << "  --submit     [value]                               \n"
    << std::endl;
}

void MPRDBC::init(int argc, char **argv)
{
    if (argc <= 1)
    {
        arguments_.showHelp = true;
        return;
    }

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        arguments_.showHelp = true;
        return;
    }
    else if (!strcmp(argv[1], "register") ||
                !strcmp(argv[1], "delete") ||
                !strcmp(argv[1], "import") ||
                !strcmp(argv[1], "config") ||
                !strcmp(argv[1], "commit"))
    {
        arguments_.opera = std::string(argv[1]);
    }
    else throw std::runtime_error("error: missing operation");

    for (int argN = 2; argN < argc; ++argN)
    {
        std::string option = {argv[argN]};

        if (option == "-h" || option == "--help")
        {
            arguments_.showHelp = true;
            break;
        }
        else if (option == "--key")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.key.first = true;
            arguments_.key.second = argv[argN];
        }
        else if (option == "--player")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.player.first = true;
            arguments_.player.second = argv[argN];
        }
        else if (option == "--remote")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.remote.first = true;
            arguments_.remote.second = argv[argN];
        }
        else if (option == "--comment")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.comment.first = true;
            arguments_.comment.second = argv[argN];
        }
        else if (option == "--banList")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.banList.first = true;
            arguments_.banList.second = argv[argN];
        }
        else if (option == "--submit")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.submit.first = true;
            arguments_.submit.second = argv[argN];
        }
        else if (option == "--name")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.name.first = true;
            arguments_.name.second = argv[argN];
        }
        else
            throw std::runtime_error("error: invalid use of option: " + option);
    }

    //Check directory ${HOME}/.config/mprdbc

    std::string directory(home_dir_);
    directory.append("/.config");

    mkdir(directory.c_str(),0700);
    directory.append("/mprdbc");
    mkdir(directory.c_str(),0700);
    if (errno != 0 && errno != 17)
        throw std::runtime_error("error: create directory fail");

}

void MPRDBC::GenerateDefaultGlobalConfig()
{
    std::cout
    << "# type \"sudo mprdbc config > /etc/mprdbc.conf\" in shell to deploy configuration\n"
    << "# program will load configuration file in the following order\n"
    << "# mprdbc.conf\n"
    << "# ${home}/.config/mprdbc/mprdbc.conf\n"
    << "# /etc/mprdbc.conf\n"
    << std::endl;
}

void MPRDBC::printMissingConfig()
{
    std::cout
    << "error: missing config file,type \"mprdbc config\" for help"
    << std::endl;
}

void MPRDBC::CheckKey()
{
    if (!arguments_.key.first)
    {
        if (user_config_[""]["key"].empty())
        {
            std::string reason;
            reason += "error: missing key_id \n";
            reason += "use gpg --gen-key to generate gpg key \n";
            reason += "use gpg --fingerprint to get key id \n";
            reason += "key_id is the last 16 characters of fingerprint";
            throw std::runtime_error(reason);
        }
        else
        {
            arguments_.key.first = true;
            arguments_.key.second = user_config_[""]["key"];
            std::cout << "Load key_id from "
            << user_config_.getFilePath() << std::endl;
        }
    }
    else
    {
        arguments_.key.first = true;
        user_config_[""]["key"] = arguments_.key.second;
        user_config_.save();
        std::cout << "Save key_id to " << user_config_.getFilePath() << std::endl;
    }
}

void MPRDBC::launch()
{
    //print help
    if (arguments_.showHelp)
    {
        printHelp();
        return;
    }

    if (arguments_.opera == "config")
    {
        GenerateDefaultGlobalConfig();
        return;
    }

    if (arguments_.opera == "register")
    {
        CheckKey();

        if (!arguments_.remote.first)
        {
            throw std::runtime_error("error: missing argument: --remote");
        }

        if (!arguments_.name.first)
        {
            throw std::runtime_error("error: missing argument: --name");
        }

        std::string url = arguments_.remote.second;
        url.append("/v1/server/register");

        Value value;
        GPGTool tool(arguments_.key.second.c_str());
        value["public_key"] = tool.getKey();
        value["message"] = tool.sign("server_name: " + arguments_.name.second);

        connection.setMethod(kPut);
        connection.setUrl(url.c_str());
        connection.setMessage(value.transform().c_str(),kJson);

        Value reply = Value::CreateValueByJson(connection.connect());
        if (reply["status"].transform() == R"("NG")")
        {
            std::string reason;
            reason += "register fail : ";
            reason += reply["reason"].transform();
            throw std::runtime_error(reason);
        }
        else
        {
            std::cout << "register success" << std::endl;

            user_config_[""]["remote_server"] = arguments_.remote.second;
            user_config_.save();

            std::cout << "Save remote server to " << user_config_.getFilePath()
            << std::endl;
        }
        return;
    }

    if (arguments_.opera == "commit")
    {

    }
}

MPRDBC::MPRDBC()
{
    //Check config
    home_dir_ = getenv("HOME");
    global_config_.setFilePath("mprdbc.conf");
    if (!global_config_.isFileExit())
    {
        global_config_.setFilePath(home_dir_ + "/.config/mprdbc/mprdbc.conf");
    }

    if (!global_config_.isFileExit())
    {
        global_config_.setFilePath("/etc/mprdbc.conf");
    }

    user_config_.setFilePath(home_dir_ + "/.config/mprdbc/config");
}
