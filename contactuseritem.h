#ifndef CONTACTUSERITEM_H
#define CONTACTUSERITEM_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"

/***********************************************
    * @file     contactuseritem.h
    * @brief    联系人列表中的联系人item
    *
    * @author   wmlatiao
    * @date     2024/12/10
    * @history
***********************************************/

namespace Ui {
class ContactUserItem;
}

class ContactUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ContactUserItem(QWidget *parent = 0);
    ~ContactUserItem();
    QSize sizeHint();          // 返回控件默认大小
    void setInfo(std::shared_ptr<AuthInfo> authInfo);
    void setInfo(std::shared_ptr<AuthRsp> authResp);
    void setInfo(int uid, QString name, QString icon);
    std::shared_ptr<UserInfo> getInfo();
    void showRedPoint(bool show = false);

private:
    Ui::ContactUserItem *ui;
    std::shared_ptr<UserInfo> _userInfo;
};

#endif // CONTACTUSERITEM_H
