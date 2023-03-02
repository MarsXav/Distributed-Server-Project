#ifndef LOGININFOINSTANCE_H
#define LOGININFOINSTANCE_H

#include <QString>

class LoginInfoInstance
{
public:
    static LoginInfoInstance* getInstance();
    static void destroy();

    void setLoginInfo(QString user, QString ip, QString port, QString token = "");
    QString getUser() const;
    QString getIP() const;
    QString getPort() const;
    QString getToken() const;

private:
    LoginInfoInstance();
    LoginInfoInstance(const LoginInfoInstance&);

    class Garbo
    {

    public:
        ~Garbo()
        {
            LoginInfoInstance::destroy();
        }
    };

    static Garbo tmp;
    static LoginInfoInstance* instance;

private:
    QString user;
    QString token;
    QString ip;
    QString port;
};

#endif // LOGININFOINSTANCE_H
