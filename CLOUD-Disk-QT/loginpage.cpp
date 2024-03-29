#include "loginpage.h"
#include "ui_loginpage.h"
#include "logininfoinstance.h"

#include <QPainter>
#include <qpushbutton.h>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDir>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>	// process json objectj
#include <QJsonArray>	// process json array
#include <QJsonValue>	// pack data

LoginPage::LoginPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    // remove the frame created
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //set universal font
    this->setFont(QFont("Arial Black", 9, false));

    // process title widget signal
    connect(ui->Title, &Titlewg::closeWindow, this, [=](){
        // identify current stacked Widget page
            this->close();
    });

    connect(ui->Title, &Titlewg::showSetWindow, this, [=](){
        if (ui->stackedWidget->currentWidget() == ui->Set_pg_2){
            ui->stackedWidget->setCurrentWidget(ui->Login_pg);
        } else {
            ui->stackedWidget->setCurrentWidget(ui->Set_pg_2);
        }
    });

    // process Register page
    connect(ui->GoToRegister, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->Reg_pg_2);
    });

    connect (ui->back_btn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->Login_pg);
    });

    connect (ui->setting_go_back, &QPushButton::clicked, this,[=](){
        ui->stackedWidget->setCurrentWidget(ui->Login_pg);
    });
}

LoginPage::~LoginPage()
{
    delete ui;
}

//void LoginPage::paintEvent(QPaintEvent* event)
//{
//   // paint background for the window
//    QPainter p(this);
//    QPixmap pixmap("://rsc/simple-bright-sky-diver-4dk2h4vfpwcjtifj.jpg");
//    p.drawPixmap(0,0,this->width(), this->height(), pixmap);
//}

bool LoginPage::RegCheck()
{
    QString userName = ui->reg_userid->text();
    QString password = ui->reg_password->text();
    QString confirm = ui->reg_confirm->text();
    QString phone = ui->reg_phone->text();
    QString mail = ui->reg_email->text();

    // obtain user data from input
    QRegularExpression regexp;
    QString USER_REG = "^[a-zA-Z0-9_@#-\\*]\\{3,16\\}$";
    regexp.setPattern(USER_REG);
    if (!regexp.match(userName).hasMatch()){
        QMessageBox::warning(this, "ERROR", "Incorrect username format");
        return false;
    }
    QString PASS_REG = "^[a-zA-Z0-9_@#-\\*]\\{6-18\\}$";
    regexp.setPattern(PASS_REG);
    if (!regexp.match(password).hasMatch()) {
        QMessageBox::warning(this, "ERROR", "Incorrect password format");
        return false;
    }
    if (password != confirm) {
        QMessageBox::warning(this, "ERROR", "Unidentical passwords");
        return false;
    }
    QString PHONE_REG = "1\\d\\{10\\}";
   regexp.setPattern(PHONE_REG);
   if (!regexp.match(phone).hasMatch()) {
       QMessageBox::warning(this, "ERROR", "Incorrect phone format");
       return false;
   }
   QString EMAIL_REG = "^[a-zA-Z\\d\\._]\\+#[a-zA-Z\\d_\\.-]\\+(\\.[a-zA-Z0-9_-]\\+)+$";
   regexp.setPattern(EMAIL_REG);
    if (!regexp.match(mail).hasMatch()) {
       QMessageBox::warning(this, "ERROR", "Incorrect email format");
       return false;
   }
    return true;
}

void LoginPage::on_Reg_btn_clicked()
{
    // process
    // data verification
    if (!LoginPage::RegCheck()) {
        return;
    }
    QString userName = ui->reg_userid->text();
    QString password = ui->reg_password->text();
    QString confirm = ui->reg_confirm->text();
    QString phone = ui->reg_phone->text();
    QString mail = ui->reg_email->text();

    if (confirm != password)
    {
       QMessageBox::warning(this, "Warning", "Two passwords are not identical");
       return;
    }
    // send data to server
   // join data to json object
    QStringList keys = QStringList() << "userName" << "password" << "confirm" << "phone" << "mail";
    QStringList values = QStringList() << userName << password << confirm << phone << mail;    // send data thru http protocal, POST
    // format: json object
    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest* request = nullptr;
    QString url = QString("http://%1:%2/reg").arg(ui->server_ip->text()).arg(ui->server_port->text());
    request->setUrl(url);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); //describe post data format

    // format a json object string
    QJsonObject obj;
    obj.insert("userName", userName);
    obj.insert("password", QJsonValue(password));
    obj.insert("confirm", QJsonValue(userName));
    obj.insert("phone", QJsonValue(userName));
    obj.insert("email", QJsonValue(mail));

    //obj->doc
    QJsonDocument doc(obj);

    //doc->array
    QByteArray json = doc.toJson();
    qDebug() << "register json data" << json;
    request->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request->setHeader(QNetworkRequest::ContentLengthHeader, QVariant(json.size()));
    QNetworkReply* reply = pManager->post(*request, json);

    // receive response data
    connect(reply, &QNetworkReply::readyRead, this, [=]{
        // analyse server response
        // receive data
        QByteArray all = reply->readAll();
        // format of server reply -> parsing
        QJsonDocument doc = QJsonDocument::fromJson(all);
        //doc->obj
        QJsonObject myobj = doc.object();
        QString status = myobj.value("code").toString();

        // on success, alert client
        if ("002" == status) {
            QMessageBox::information(this, "Successful registration", "Succesful registration, proceed to login");

            // clear line edit
            ui->reg_userid->clear();
            ui->reg_password->clear();
            ui->reg_confirm->clear();
            ui->reg_email->clear();
            ui->reg_phone->clear();

            //set login page info
            ui->login_id->setText(userName);
            ui->login_password->setText(password);

            //switch to login page
            ui->stackedWidget->setCurrentWidget(ui->Login_pg);
        }
        // on fail
        else if ("003" == status) {
            QMessageBox::warning(this, "ERROR", "User already exits");
        } else {
            QMessageBox::warning(this, "ERROR", "Error");
        }
    });
    // process response data
}

void LoginPage::on_sign_in_btn_clicked()
{
    QString id = ui->login_id->text();
    QString ip = ui->server_ip->text();
    QString port = ui->server_port->text();
    QString pw = ui->login_password->text();
    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest* request = nullptr;
    QString url = QString("http://%1:%2/login").arg(ui->server_ip->text()).arg(ui->server_port->text());
    request->setUrl(url);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj.insert("userID", id);
    obj.insert("password", pw);
    //obj->doc
    QJsonDocument doc(obj);
    //doc->array
    QByteArray json = doc.toJson();

    QNetworkReply* reply = pManager->post(*request, json);
    // receive response data
    connect(reply, &QNetworkReply::readyRead, this, [=]{
        // analyse server response
        // receive data
        QByteArray all = reply->readAll();
        // format of server reply -> parsing
        QJsonDocument doc = QJsonDocument::fromJson(all);
        //doc->obj
        QJsonObject myobj = doc.object();
        QString status = myobj.value("code").toString();

        // on success, alert client
        if ("000" == status)
        {
            cout << "Successfully login";
            // setup login info
            LoginInfoInstance *p = LoginInfoInstance::getInstance();
            p->setLoginInfo(id, ip, port, obj.value("token").toString());

            // hide current window
            this->hide();
            // switch to main window

        }
        // on fail
        else if ("001" == status)
        {
            QMessageBox::warning(this, "ERROR", "Wrong password or user ID");
        } else
        {
            QMessageBox::warning(this, "ERROR", "Error");
        }
    });
}

void LoginPage::on_setting_confirm_btn_clicked()
{
    QString ip = ui->server_ip->text();
    QString port = ui->server_port->text();
    m_cm.writeWebInfo(ip, port);
}

