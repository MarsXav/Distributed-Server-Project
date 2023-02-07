#include "titlewg.h"
#include "ui_titlewg.h"
#include <QMouseEvent>
#include <QPushButton>

Titlewg::Titlewg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Titlewg)
{
    ui->setupUi(this);

    // set static pic
    // logo
    ui->logo->setPixmap(QPixmap("://rsc/icons8-itunes-store-16.png").scaled(24,24));
    ui->app_name->setStyleSheet("color:gray");
    m_parent = parent;

    //buttons realisation
    connect(ui->set_btn, &QPushButton::clicked, this, [=](){
        // send custom signal
        emit showSetWindow();
    });

    connect(ui->min_btn, &QPushButton::clicked, this, [=](){
        m_parent->showMinimized();
    });

    connect(ui->close_btn, &QPushButton::clicked, this, [=](){
        emit closeWindow();
    });
}

Titlewg::~Titlewg()
{
    delete ui;
}

void Titlewg::mouseMoveEvent(QMouseEvent *event)
{
    // only allow left hold drag
    if (event->buttons() & Qt::LeftButton) {
        // window follows curser
        m_parent->move(event->globalPos()-m_pt);
    }
}

void Titlewg::mousePressEvent(QMouseEvent *event)
{
    // if left hold
    if (event->button() == Qt::LeftButton)
    {
        // derive the differnece
        m_pt = event->globalPos() - m_parent->geometry().topLeft();
    }
}
