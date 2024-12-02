#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>

namespace Ui {
class StateWidget;
}

class StateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StateWidget(QWidget *parent = 0);
    ~StateWidget();

private:
    Ui::StateWidget *ui;
};

#endif // STATEWIDGET_H
