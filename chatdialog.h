#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"

/***********************************************
    * @file     chatdialog.h
    * @brief    聊天界面
    *
    * @author   wmlatiao
    * @date     2024/11/20
    * @history
***********************************************/

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
    void addUserListTest(); // 测试添加用户列表的功能

private:
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    bool _b_loading;

    void showList(ChatUIMode mode, bool search=false);     // 显示当前模式的一个界面，search代表是否是搜索状态，决定是否显示搜索框
signals:

public slots:
    void slot_loading_chat_user();  // 在聊天列表滚动到底时加载更多联系人信息

};

#endif // CHATDIALOG_H
