#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include "statewidget.h"
#include <QMouseEvent>
#include "userdata.h"
#include <QListWidgetItem>

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
protected:
    bool eventFilter(QObject* watched, QEvent* event)override;

private:
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    bool _isSearching;               // 用于记录当前是否处于搜索状态
    bool _b_loading;
    QList<StateWidget*> _lbList;  // 存放侧边栏控件的组
    QMap<int, QListWidgetItem*>_chat_items_added;           // 已经添加了的好友聊天条目
    QMap<int, QListWidgetItem*> _contact_items_added;       // 已经添加了的联系人条目
    int _cur_chat_uid;                                      // 记录当前选中的聊天条目对应的uid（表示当前正在和此uid用户聊天）
    QWidget* _lastWidget;                                   // 记录最近一次的界面信息

    void addUserListTest(); // 测试添加用户列表的功能
    void addUserList();     // 往聊天列表添加条目
    void addContactList();  // 往联系人列表添加条目
    void showList(ChatUIMode mode, bool search=false);      // 显示当前模式的一个界面，search代表是否是搜索状态，决定是否显示搜索框
    void addLabelGroup(StateWidget* label);                 // 将侧边栏按钮加到一个组里方便管理
    void handleGlobalMousePress(QMouseEvent* event);        // 在事件过滤器中处理鼠标点击按下事件的函数
    void setSelectChatItem(int uid=0);                        // 设置选中的聊天条目
    void setSelectChatPage(int uid=0);                        // 设置选中的聊天页面
    void updateChatMsg(const std::vector<std::shared_ptr<TextChatData>>& msgdata);  // 根据消息列表更新聊天内容
signals:

public slots:
    void slot_loading_chat_user();              // 在聊天列表滚动到底时加载更多聊天信息
    void slot_loading_contact_user();           // 在联系人列表滚动到底时加载更多联系人信息
    void slot_sideWid_clicked(StateWidget* w);  // 点击侧边栏时做一个切换的效果
    void slot_text_changed(const QString&);     // 当搜索框内容改变时显示搜索结果的预览
    void slot_apply_friend(std::shared_ptr<AddFriendApply> applyInfo);  // TcpMgr收到新好友申请时的处理
    void slot_add_auth_firend(std::shared_ptr<AuthInfo> authInfo);      // 对端同意自己的好友申请后更新界面
    void slot_auth_rsp(std::shared_ptr<AuthRsp>);                       // 同意对端的好友申请后更新界面
    void slot_jump_chat_item_from_search(std::shared_ptr<SearchInfo> si);           // 搜索到好友，跳转到好友聊天界面
    void slot_jump_chat_item_from_infoPage(std::shared_ptr<UserInfo> ui);             // 在联系人列表点击文字聊天,跳转到好友聊天界面
    void slot_switch_friend_info_page(std::shared_ptr<UserInfo> userInfo);                                // 在联系人列表点击联系人条目，打开联系人详情界面
    void slot_switch_apply_friend_page();                               // 在联系人列表点击新朋友项，打开收到的好友申请界面
    void slot_chatList_item_clicked(QListWidgetItem* item);             // 在聊天列表点击聊天项，打开聊天界面
    void slot_append_send_chat_msg(std::shared_ptr<TextChatData> chatData);         // 向对应的UserInfo里添加消息
    void slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg);          // 收到好友消息，更新界面

};

#endif // CHATDIALOG_H
