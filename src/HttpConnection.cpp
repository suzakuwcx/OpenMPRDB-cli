#include "HttpConnection.h"
#include "shell.hpp"

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
    std::vector<std::string> params;
    params.emplace_back("--silent");
    params.emplace_back("-H");
    if (send_msg_.second == kJson)
        params.emplace_back("Content-Type: application/json");
    else if (send_msg_.second == kPlain)
        params.emplace_back("Content-Type: text/plain");
    else
        throw std::runtime_error("illegal message");

    switch (method_)
    {
        case kGet:
            params.emplace_back("-X");
            params.emplace_back("GET");
            break;
        case kPost:
            params.emplace_back("-X");
            params.emplace_back("POST");
            params.emplace_back("-d");
            params.emplace_back(send_msg_.first);
            break;
        case kPut:
            params.emplace_back("-X");
            params.emplace_back("PUT");
            params.emplace_back("-d");
            params.emplace_back(send_msg_.first);
            break;
        case kDelete:
            params.emplace_back("-X");
            params.emplace_back("DELETE");
            params.emplace_back("-d");
            params.emplace_back(send_msg_.first);
            break;
    }
    params.emplace_back(url_);
    return execute("curl",params);
}
