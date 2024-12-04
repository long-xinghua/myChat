#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include "statewidget.h"
#include <QMouseEvent>

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
protected:
    bool eventFilter(QObject* watched, QEvent* event)override;

private:
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    bool _isSearching;               // 用于记录当前是否处于搜索状态
    bool _b_loading;
    QList<StateWidget*> _lbList;  // 存放侧边栏控件的组

    void showList(ChatUIMode mode, bool search=false);      // 显示当前模式的一个界面，search代表是否是搜索状态，决定是否显示搜索框
    void addLabelGroup(StateWidget* label);                 // 将侧边栏按钮加到一个组里方便管理
    void handleGlobalMousePress(QMouseEvent* event); // 在事件过滤器中处理鼠标点击按下事件的函数
signals:

public slots:
    void slot_loading_chat_user();              // 在聊天列表滚动到底时加载更多联系人信息
    void slot_sideWid_clicked(StateWidget* w);  // 点击侧边栏时做一个切换的效果
    void slot_text_changed(const QString&);     // 当搜索框内容改变时显示搜索结果的预览

};

#endif // CHATDIALOG_H
