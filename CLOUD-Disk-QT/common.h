#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <QString>
#include <QWidget>
#include <QListWidgetItem>
#include <QNetworkAccessManager>

#define RECORDDIR "conf/record/"
#define FILETYPEDIR "conf/fileType"
#define CONFFILE "conf/cfg.json"

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__<< "]"


class common : public QObject
{
public:
    common(QObject* parent = 0);
    ~common(){};

    void writeRecord(QString user, QString name, QString code, QString path = RECORDDIR);

    void writeWebInfo(QString ip, QString port, QString path = CONFFILE);

    // obtain corresponding parameters from conf
    QString getCfgValue(QString title, QString key, QString path = CONFFILE);

private:
    static QString m_typePath;
};

#endif // COMMON_H
