#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDialog>

namespace Ui {
class LoginPage;
}

class LoginPage : public QDialog
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

protected:
//    void paintEvent(QPaintEvent* event);
    bool RegCheck();

private slots:
    void on_Reg_btn_clicked();

    void on_sign_in_btn_clicked();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
