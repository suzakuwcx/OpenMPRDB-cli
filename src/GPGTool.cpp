#include "GPGTool.h"
#include "shell.hpp"

#include <iostream>

GPGTool::GPGTool(const char * key_id) : key_id_(key_id) {}

GPGTool::~GPGTool() = default;

std::string GPGTool::getKey()
{
    Check();
    std::vector<std::string> params;
    params.emplace_back("-a");
    params.emplace_back("--export");
    params.emplace_back(key_id_);

    return execute("gpg",params);
}

std::string GPGTool::sign(const std::string& message)
{
    Check();
    std::vector<std::string> params;
    std::string passphrase;
    std::cout << "enter your passphrase:";
    std::cin >> passphrase;
    params.emplace_back("--clearsign");
    params.emplace_back("--local-user");
    params.emplace_back(key_id_);
    params.emplace_back("--pinentry-mode");
    params.emplace_back("loopback");
    params.emplace_back("--batch");
    params.emplace_back("--passphrase");
    params.emplace_back(passphrase);
    //echo "$message" | gpg --clearsign --local-user ${key_id} --batch --passphrase xx20000726 --pinentry-mode loopback
    return execute("gpg",params,message);
}

void GPGTool::Check()
{
    if (key_id_.empty())
        throw std::runtime_error("missing gpg key_id");
}
