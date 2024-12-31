#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <memory>
#include "userdata.h"
#include "usermgr.h"

/***********************************************
    * @file     chatpage.h
    * @brief    界面右侧stackWidget中的聊天页面
    *
    * @author   wmlatiao
    * @date     2024/11/25
    * @history
***********************************************/

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = 0);
    ~ChatPage();
    void setUserInfo(std::shared_ptr<UserInfo> userInfo);
    void appendChatMsg(std::shared_ptr<TextChatData> msg);     // 向chatView聊天区域中添加对话

protected:
    void paintEvent(QPaintEvent* event) override;   // 重写paintEvent来设置背景颜色等，因为Qt没在QWidget里给接口来实现比较高级的喷绘功能

private:
    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> _userInfo;
signals:
    void sig_append_send_chat_msg(std::shared_ptr<TextChatData> text_msg);      // 向msgWidget中添加消息

private slots:
    void on_sendButton_clicked();
};

#endif // CHATPAGE_H
