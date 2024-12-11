#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"
#include <memory>

/***********************************************
    * @file     applyfrienditem.h
    * @brief    添加好友页面中的列表item
    *
    * @author   wmlatiao
    * @date     2024/12/11
    * @history
***********************************************/

namespace Ui {
class ApplyFriendItem;
}

class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ApplyFriendItem(QWidget *parent = 0);
    ~ApplyFriendItem();
    void setInfo(std::shared_ptr<ApplyInfo> applyInfo); // 获取applyInfo并设置item控件信息
    void showAddBtn(bool show);
    QSize sizeHint()const override;     // 返回默认控件大小
    int getUid();

private:
    Ui::ApplyFriendItem *ui;
    std::shared_ptr<ApplyInfo> _applyInfo;
    bool _added;
signals:
    void sig_auth_friend(std::shared_ptr<ApplyInfo> applyInfo);
};

#endif // APPLYFRIENDITEM_H
