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
protected:
    virtual void paintEvent(QPaintEvent *event) override;   // 使用这个函数会导致好友列表里的条目没有悬浮和选中效果，不知道为啥     已修复
private:
    ListItemType _itemType;
public slots:


signals:

};

#endif // LISTITEMBASE_H
