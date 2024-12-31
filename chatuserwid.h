#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"

/***********************************************
    * @file     chatuserwid.h
    * @brief    显示聊天用户的widget, 从继承自QWidget修改成继承自ListItemBase
    *
    * @author   wmlatiao
    * @date     2024/11/21
    * @history
***********************************************/

namespace Ui {
class ChatUserWid;
}


class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = 0);
    ~ChatUserWid();

    QSize sizeHint() const override;    // 返回自定义的尺寸
    void setInfo(std::shared_ptr<UserInfo> user_info);
    std::shared_ptr<UserInfo> getUserInfo();
    void updateLastMsg(const std::vector<std::shared_ptr<TextChatData>>& msgs); // 通过新接收的msgs更新_user_info信息和最近聊天信息

private:
    Ui::ChatUserWid *ui;
    std::shared_ptr<UserInfo> _user_info;
};

#endif // CHATUSERWID_H
