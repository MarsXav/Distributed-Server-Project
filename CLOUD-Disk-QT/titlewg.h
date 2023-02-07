#ifndef TITLEWG_H
#define TITLEWG_H

#include <QWidget>

namespace Ui {
class Titlewg;
}

class Titlewg : public QWidget
{
    Q_OBJECT

public:
    explicit Titlewg(QWidget *parent = nullptr);
    ~Titlewg();

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::Titlewg *ui;
    QPoint m_pt; // difference bbetween current position of curser and point on left upper corner

    QWidget* m_parent;

signals:
    void showSetWindow();
    void showMinWindow();
    void closeWindow();
};

#endif // TITLEWG_H
