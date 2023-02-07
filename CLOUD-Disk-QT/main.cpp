#include "mainwindow.h"

#include <QApplication>

#include "loginpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginPage w;
    w.show();
    return a.exec();
}
