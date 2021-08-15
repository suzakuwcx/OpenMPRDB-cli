#include "GPGTool.h"
#include "shell.h"

#include <iostream>

GPGTool::GPGTool(const char * key_id) : key_id_(key_id) {}

GPGTool::~GPGTool() = default;

std::string GPGTool::getKey()
{
    std::string command = "gpg -a --export ";
    command += key_id_;
    return execute(command.c_str());
}

std::string GPGTool::sign(std::string message)
{
    std::string command;
    std::string passphrase;
    std::cout << "enter your passphrase:";
    std::cin >> passphrase;
    //echo "$message" | gpg --clearsign --local-user ${key_id} --batch --passphrase xx20000726 --pinentry-mode loopback
    command = command + "echo \"" + message + "\" | gpg --clearsign --local-user " +
    key_id_ + " --pinentry-mode loopback --batch --passphrase " + passphrase;
    return execute(command.c_str());
}
