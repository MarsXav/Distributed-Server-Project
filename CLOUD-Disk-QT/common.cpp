#include "common.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

common::common(QObject* parent)
{
    Q_UNUSED(parent);
}

QString common::m_typePath = FILETYPEDIR;

void writeRecord(QString user, QString name, QString code, QString path = RECORDDIR)
{
    QString fileName = path + user;
    QDir dir(path);
    cout << "path" << path;
    if (!dir.exists()) // if directory does not exit
    {
        if (dir.mkpath(path))
        {
            cout << path << "Directory succesfully created";
        }
        else
        {
            cout << path << "Directory failed to create";
        }
    }
    cout << "fileName = " << fileName.toUtf8().data();
    QByteArray array;
    QFile file(fileName);

    // if file existed, read file content
    if (file.exists() == true)
    {
        if (file.open(QIODevice::ReadOnly))
        {
            cout << "file.open(QIODevice::ReadOnly) error";
            return;
        }
    }
    array = file.readAll();
    file.close();

    if (file.open(QIODevice::WriteOnly))
    {
        cout << "file.open(QIODevice::WriteOnly) error";
        return;
    }
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss ddd");
}

QString common::getCfgValue(QString title, QString key, QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        cout << "file open error";
        return QString();
    }
    QByteArray json = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    if (doc.isNull() || doc.isEmpty())
    {
        cout << "doc.isNull || doc.isEmpty";
        return QString();
    }
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        QJsonObject temp = obj.value(title).toObject();
        QStringList list = temp.keys();
        for (int i = 0; i < list.size(); i++){
            if (list.at(i) == key)
            {
                return temp.value(list.at(i)).toString();
            }
        }
    }
    return QString();
}

void common::writeWebInfo(QString ip, QString port, QString path)
{
    // webserver data
    QMap<QString, QVariant> web_server;
    web_server.insert("ip", ip);
    web_server.insert("port", port);
    // type_path data
    QMap<QString, QVariant> type_path;
    type_path.insert("path", m_typePath);
    // login info
    QString user = getCfgValue("login", "user");
    QString pwd = getCfgValue("login", "pwd");
    QString remember = getCfgValue("login", "remember");

    QMap<QString, QVariant> login;
    login.insert("user", user);
    login.insert("pwd", pwd);
    login.insert("remember", remember);

    QMap<QString, QVariant> json;
    json.insert("web_server", web_server);
    json.insert("type_path", type_path);
    json.insert("login", login);
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(json);

    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        cout << "file open err";
        return;
    }
    file.write(jsonDocument.toJson());
    file.close();
}
