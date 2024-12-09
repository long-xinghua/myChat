#ifndef CLICKONCELABEL_H
#define CLICKONCELABEL_H
#include <QLabel>
#include <QMouseEvent>

/***********************************************
    * @file     clickoncelabel.h
    * @brief    能够点击一次并有相应效果的label
    *
    * @author   wmlatiao
    * @date     2024/12/06
    * @history
***********************************************/

class ClickOnceLabel : public QLabel
{
    Q_OBJECT
public:
    ClickOnceLabel(QWidget* parent=nullptr);
protected:
    // void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
private:
signals:
    void clicked(QString);

};

#endif // CLICKONCELABEL_H
