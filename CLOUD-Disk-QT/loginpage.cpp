#include "loginpage.h"
#include <QPainter>
#include "qpushbutton.h"
#include "ui_loginpage.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

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
        if (ui->stackedWidget->currentWidget() == ui->Set_pg_2)
        {
            // switch to login page
            ui->stackedWidget->setCurrentWidget(ui->Login_pg);
        }
        else if (ui->stackedWidget->currentWidget() == ui->Reg_pg_2)
        {
            ui->stackedWidget->setCurrentWidget(ui->Login_pg);
        }
        else
        {
            this->close();
        }
    });

    connect(ui->Title, &Titlewg::showSetWindow, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->Set_pg_2);
    });

    // process Register page
    connect(ui->GoToRegister, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->Reg_pg_2);
    });

    connect (ui->back_btn, &QPushButton::clicked, this, [=](){
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
    // send data to server
   // join data to json object
    // send data thru http protocal, POST
    // format: json object
    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest* request;
    request->setUrl(http://192.168.101.123/reg);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); //describe post data format
    QNetworkReply* reply = pManager->post(request, data);
    // receive response data
    connect(reply, &QNetworkReply::readyRead, this, [=]{
        // analyse server response
        // receive data
        QByteArray all = reply->readAll();
        // format of server reply -> parsing
        // obtain
    });
    // process response data
}

