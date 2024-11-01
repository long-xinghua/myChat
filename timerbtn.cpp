#include "timerbtn.h"
#include <QMouseEvent>
#include <QDebug>

TimerBtn::TimerBtn(QWidget *parent):QPushButton(parent), _counter(10)    // 构造函数初始化时先把基类QPushButton构造好
{
    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, [this](){
        _counter--;
        if(_counter <= 0){
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            this->setEnabled(true); // 将按钮恢复到可用状态
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn()
{
    _timer->stop(); // 由于会先析构TimerBtn对象再析构_timer，所以在析构函数中先将_timer停止
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        qDebug()<<"鼠标左键释放";
        this->setEnabled(false);    // 将按钮设为不可用状态
        this->setText(QString::number(_counter));
        _timer->start(1000);    // 计时器定时1秒
        emit clicked(); // 由于重写了mouseReleaseEvent事件，可能需要手动发送clicked信号外部才能知道按钮被点击了，有些版本的编译器不需要写这个
    }
    // 再调用一下基类的释放鼠标事件，确保一些默认行为能正常执行，还能保证clicked()信号不会重复发送
    QPushButton::mouseReleaseEvent(event);

}
