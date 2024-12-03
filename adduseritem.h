#ifndef ADDUSERITEM_H
#define ADDUSERITEM_H

#include <QWidget>
#include "listitembase.h"


/***********************************************
    * @file     adduseritem.h
    * @brief    显示添加好友的widget, 从继承自QWidget修改成继承自ListItemBase
    *
    * @author   wmlatiao
    * @date     2024/12/03
    * @history
***********************************************/

namespace Ui {
class AddUserItem;
}

class AddUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit AddUserItem(QWidget *parent = 0);
    ~AddUserItem();
    QSize sizeHint() const override;    // 返回自定义的尺寸
private:
    Ui::AddUserItem *ui;
};

#endif // ADDUSERITEM_H
