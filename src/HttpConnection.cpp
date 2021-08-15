#include "HttpConnection.h"
#include "shell.h"

HttpConnection::HttpConnection() : method_(kGet)
{
    send_msg_.second = kPlain;
}

HttpConnection::~HttpConnection() = default;

std::string HttpConnection::getMessage() const
{
    return send_msg_.first;
}

std::string HttpConnection::getUrl() const
{
    return url_;
}

Method HttpConnection::getMethod() const
{
    return method_;
}

void HttpConnection::setMethod(Method method)
{
    method_ = method;
}

void HttpConnection::setMessage(const char *msg,Type type)
{
    send_msg_.first = msg;
    send_msg_.second = type;
}

void HttpConnection::setUrl(const char *url)
{
    url_ = url;
}

std::string HttpConnection::connect()
{
    std::string command = "curl";
    if (send_msg_.second == kJson)
        command += " -H 'Content-Type: application/json'";
    switch (method_)
    {
        case kGet:
            command += " -X GET";
            break;
        case kPost:
            command += " -X POST";
            command += " -d \'";
            command += send_msg_.first;
            command.append("\'");
            break;
        case kPut:
            command += " -X PUT";
            command += " -d \'";
            command += send_msg_.first;
            command.append("\'");
            break;
        case kDelete:
            command += " -X DELETE";
            command += " -d \'";
            command += send_msg_.first;
            command.append("\'");
            break;
    }

    command += " ";
    command += url_;
    return execute(command.c_str());
}
