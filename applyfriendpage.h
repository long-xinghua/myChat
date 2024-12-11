#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QWidget>
#include "userdata.h"
#include <memory>
#include <QJsonArray>
#include <unordered_map>
#include "applyfrienditem.h"

/***********************************************
    * @file     applyfriendpage.h
    * @brief    联系人页面最右侧的添加好友界面
    *
    * @author   wmlatiao
    * @date     2024/12/11
    * @history
***********************************************/

namespace Ui {
class ApplyFriendPage;
}

class ApplyFriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = 0);
    ~ApplyFriendPage();
    void addNewApply(std::shared_ptr<AddFriendApply> apply);
protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::ApplyFriendPage *ui;
    std::unordered_map<int, ApplyFriendItem*> _unauthItems; // 储存未同意的好友申请item

    void loadApplyList();   // 加载申请列表
public slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp> rsp);   // 点击同意对方申请后触发该槽函数
signals:
    void sig_show_search(bool);
};

#endif // APPLYFRIENDPAGE_H
