#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include "global.h"

/***********************************************
    * @file     listitembase.h
    * @brief    所有列表中物体的基类，继承自QWidget
    *
    * @author   wmlatiao
    * @date     2024/11/21
    * @history
***********************************************/


class ListItemBase: public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget* parent=nullptr);
    void setItemType(ListItemType itemType);    // 设置条目类型
    ListItemType getItemType();                 // 获取条目类型
private:
    ListItemType _itemType;
public slots:


signals:

};

#endif // LISTITEMBASE_H
