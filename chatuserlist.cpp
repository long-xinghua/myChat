#include "chatuserlist.h"

ChatUserList::ChatUserList(QWidget *parent): QListWidget(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 安装事件过滤器
    this->viewport()->installEventFilter(this); // 是给聊天列表的viewport（视窗）加过滤器而不是直接给this装
}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
{
    // 判断鼠标事件
    if(watched == this->viewport()){
        if(event->type() == QEvent::Enter){     // 判断事件是否为鼠标进入
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);    // 显示垂直滚动条
        }
        else if(event->type() == QEvent::Leave){  // 判断事件是否为鼠标离开
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏滚动条
        }
    }

    // 判断滚轮事件
    if(watched == this->viewport() && event->type() == QEvent::Wheel){
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);   // 把QEvent事件转成QWheelEvent事件
        int numDegrees = wheelEvent->angleDelta().y()/8;    //angleDelta().y()得到竖直方向上的滚动角度
        int numSteps = numDegrees / 15;                     // 计算滚动步数

        // 设置滚动条的值，把滚轮滚动的数值添加到滚动条上
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);   // 往下滚numSteps为负数，值增加，网上滚值减小

        // 取出listWidget的竖直滚动条
        QScrollBar* scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int curValue = scrollBar->value();

        if(curValue >= maxScrollValue){ // 当前值已经超过最大值，即滚动条已经到底部
            qDebug()<<"滚动条到底，加载更多用户";
            emit sig_loading_chat_user();
        }
        return true;
    }

    return QListWidget::eventFilter(watched, event);    // 其他事件还是继续走默认行为


}
