#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>

/***********************************************
    * @file     chatuserlist.h
    * @brief    聊天记录列表
    *
    * @author   wmlatiao
    * @date     2024/11/21
    * @history
***********************************************/



class ChatUserList : public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget* parent = nullptr);
protected:
    bool eventFilter(QObject *, QEvent *) override; // 捕获一些需要更改行为的事件，也能拦截事件，防止传播到目标对象
signals:
    void sig_loading_chat_user();

};

#endif // CHATUSERLIST_H
