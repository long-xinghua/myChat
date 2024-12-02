#include "listitembase.h"
#include <QStyleOption>
#include <QPainter>

ListItemBase::ListItemBase(QWidget *parent): QWidget(parent)
{

}

void ListItemBase::setItemType(ListItemType itemType)
{
    _itemType = itemType;
}

ListItemType ListItemBase::getItemType()
{
    return _itemType;
}

void ListItemBase::paintEvent(QPaintEvent *event)
{
//    QStyleOption opt;   // 定义样式选项,可以存储控件的状态、大小、启用/禁用状态等信息。
//    opt.init(this);     // 用ListItemBase的信息初始化opt
//    QPainter p(this);
//    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);    // QStyle::PE_Widget 表示要绘制一个通用小部件背景
}
