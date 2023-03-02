#include "logininfoinstance.h"
#include "common.h"

// static member, decleared in the class, defined outside the class
LoginInfoInstance::Garbo LoginInfoInstance::tmp;

LoginInfoInstance *LoginInfoInstance::getInstance()
{
    return instance;
}

void LoginInfoInstance::destroy()
{
    if (LoginInfoInstance::instance != nullptr)
    {
        delete LoginInfoInstance::instance;
        LoginInfoInstance::instance = nullptr;
        cout << "instance is deleted";
    }
}

void LoginInfoInstance::setLoginInfo(QString tempUser, QString tempIp, QString tempPort, QString token)
{
    user = tempUser;
    ip = tempIp;
    port = tempPort;
    this->token = token;
}

QString LoginInfoInstance::getUser() const
{
    return user;
}
QString LoginInfoInstance::getIP() const
{
    return ip;
}
QString LoginInfoInstance::getPort() const
{
    return port;
}
QString LoginInfoInstance::getToken() const
{
    return token;
}
