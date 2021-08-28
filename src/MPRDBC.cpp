#include <iostream>
#include "MPRDBC.h"
#include <cstring>
#include "value.h"
#include "GPGTool.h"
#include "HttpConnection.h"
#include <sys/stat.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

void MPRDBC::printHelp()
{
    std::cout << "Usage: prd-client [COMMAND] [OPTIONS] \n"
    << "Available commands: \n"
    << "  config                  Generate default config\n"
    << "  register \n"
    << "  delete \n"
    << "  submit \n"
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
                !strcmp(argv[1], "config") ||
                !strcmp(argv[1], "submit"))
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
        else if (option == "--point")
        {
            ++argN;
            if (argN >= argc || argv[argN][0] == '-')
                throw std::runtime_error("error: invalid use of option: " + option);
            arguments_.point.first = true;
            arguments_.point.second = argv[argN];
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
    << "\n"
    << "[submit]\n"
    << "# Default point when submitting , -1 means banned\n"
    << "# DefaultPoint = -1\n"
    << "# DefaultComment = banned"
    << std::endl;
}

void MPRDBC::printMissingConfig()
{
    std::cout
    << "error: missing config file,type \"mprdbc config\" for help"
    << std::endl;
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
        //manager key
        if (!arguments_.key.first)
        {
            if (user_config_[""]["key"].empty())
            {
                std::string reason;
                reason += "error: missing key_id \n";
                reason += "use \"gpg --gen-key\" to generate gpg key \n";
                reason += "use \"gpg --fingerprint\" to get key id \n";
                reason += "append \"--key [key_id]\" in \"gpg register\" \n";
                reason += "(key_id is the last 16 characters of fingerprint)";
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

        if (!arguments_.remote.first)
            throw std::runtime_error("error: missing argument: --remote");

        if (!arguments_.name.first)
            throw std::runtime_error("error: missing argument: --name");

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
            user_config_[""]["server_id"] = reply["uuid"].transform();
            user_config_.save();

            std::cout << "Save remote server to " << user_config_.getFilePath()
            << std::endl;
        }
        return;
    }

    if (arguments_.opera == "submit")
    {
        if (!arguments_.point.first)
        {
            std::cout << "Load default point from "
                        << global_config_.getFilePath() << std::endl;
            arguments_.point.first = true;
            if (global_config_["submit"]["DefaultPoint"].empty())
                arguments_.point.second = "-1";
            else
                arguments_.point.second = global_config_["submit"]["DefaultPoint"];
        }

        if (!arguments_.comment.first)
        {
            std::cout << "Load default comment from "
            << global_config_.getFilePath() << std::endl;
            arguments_.comment.first = true;
            if (global_config_["submit"]["DefaultComment"].empty())
                arguments_.comment.second = "banned";
            else
                arguments_.comment.second = global_config_["submit"]["DefaultComment"];
        }

        if (!arguments_.player.first)
            throw std::runtime_error("error: missing argument: --player");

        std::string message;
        Value value;

        boost::uuids::uuid uuid{};
        uuid = rgen_();

        value["uuid"] = to_string(uuid);
        value["timestamp"] = std::to_string(time(NULL));
        value["player_uuid"] = arguments_.player.second;
        value["points"] = arguments_.point.second;
        value["comment"] = arguments_.comment.second;

        message = message + "uuid: " + to_string(uuid) + '\n';
        message = message + "timestamp: " + std::to_string(time(NULL)) + '\n';
        message = message + "player_uuid: " + arguments_.player.second + '\n';
        message = message + "points: " + arguments_.point.second + '\n';
        message = message + "comment: " + arguments_.comment.second;

        std::string url = user_config_[""]["remote_server"];
        if (url.empty())
            throw std::runtime_error("haven't registered remote server");

        std::string key_id = user_config_[""]["key"];
        if(key_id.empty())
            throw std::runtime_error("haven't registered remote server");

        url += "/v1/submit/new";

        GPGTool tool(key_id.c_str());
        message = tool.sign(message);

        connection.setUrl(url.c_str());
        connection.setMethod(kPut);
        connection.setMessage(message.c_str(),kPlain);

        Value reply = Value::CreateValueByJson(connection.connect());
        if (reply["status"].transform() == R"("NG")")
        {
            std::string reason;
            reason += "submit fail : ";
            reason += reply["reason"].transform();
            throw std::runtime_error(reason);
        }
        else
        {
            dataBase_.addRecord(value);
            std::cout << "submit success" << std::endl;
        }
        return;
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

    dataBase_.setFilePath(home_dir_ + "/.config/mprdbc/submit");
}
