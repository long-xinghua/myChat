#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H

#include <QListWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>
#include <memory>
#include "userdata.h"



/***********************************************
    * @file     contactuserlist.h
    * @brief    自定义的联系人列表，用于chatDialog的contactList
    * 
    * @author   wmlatiao
    * @date     2024/12/10
    * @history
***********************************************/

 // 联系人列表中的物品类
class ContactUserItem;

class ContactUserList : public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget* parent = nullptr);
    void showRedPoint(bool show = true);            // 显示小红点
protected:
    bool eventFilter(QObject *, QEvent *) override; // 捕获鼠标进入、离开和滚动事件
private:
    ContactUserItem* _add_friend_item;              // 点击来添加好友的一个item
    QListWidgetItem* _groupItem;                    // ContactUserItem的基类，表示一个组的item类型
    
    void addContactUserList();                      // 往用户列表添加内容？
    
public slots:
    void slot_item_clicked(QListWidgetItem* item);
//    void slot_add_auth_friend(std::shared_ptr<AuthInfo>);   // 给别人发的好友申请
//    void slot_auth_rsp(std::shared_ptr<AuthRsp>);           // 别人发过来的好友申请
signals:
    void sig_loading_contact_user();
    void sig_switch_apply_friend_page();    // 切换到申请好友页面
    void sig_switch_friend_info_page();     // 切换到好友信息页面
};

#endif // CONTACTUSERLIST_H
