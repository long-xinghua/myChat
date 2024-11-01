#ifndef TIMERBTN_H
#define TIMERBTN_H
#include <QPushButton>
#include <QTimer>

/***********************************************
    * @file     timerbtn.h
    * @brief    有定时功能按钮的类
    *
    * @author   wmlatiao
    * @date     2024/10/31
    * @history
***********************************************/


class TimerBtn: public QPushButton
{
public:
    TimerBtn(QWidget* parent = nullptr);
    ~TimerBtn();

private:
    QTimer* _timer;
    int _counter;

    void mouseReleaseEvent(QMouseEvent* event) override;    // 重写一下鼠标释放事件

};

#endif // TIMERBTN_H
