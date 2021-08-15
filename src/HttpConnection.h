#ifndef OPENMPRDB_CLI_HTTPCONNECTION_H
#define OPENMPRDB_CLI_HTTPCONNECTION_H

#include <string>

enum Method
{
    kGet = 0,
    kPost,
    kPut,
    kDelete
};

enum Type
{
    kJson = 0,
    kPlain
};

class HttpConnection
{
public:
    HttpConnection();
    ~HttpConnection();
    //=============================================================
    std::string getMessage() const;
    std::string getUrl() const;
    Method getMethod() const;
    //=================================================================
    void setMethod(Method method);
    void setMessage(const char *msg,Type type);
    void setUrl(const char *url);
    //=============================================================
    //make a connection and get reply
    std::string connect();
private:
    std::pair<std::string,Type> send_msg_;
    std::string url_;
    Method method_;
    //=================================================================

};

#endif //OPENMPRDB_CLI_HTTPCONNECTION_H
