#ifndef GROUPTIPITEM_H
#define GROUPTIPITEM_H

#include <QWidget>
#include <listitembase.h>

/***********************************************
    * @file     grouptipitem.h
    * @brief    联系人列表中代表组的item
    *
    * @author   wmlatiao
    * @date     2024/12/10
    * @history
***********************************************/

namespace Ui {
class GroupTipItem;
}

class GroupTipItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = 0);
    ~GroupTipItem();
    QSize sizeHint();  // 返回控件默认大小
    void setGroupTip(QString str);

private:
    Ui::GroupTipItem *ui;
    QString _tip;
};

#endif // GROUPTIPITEM_H
