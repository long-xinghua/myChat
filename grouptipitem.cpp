#include "grouptipitem.h"
#include "ui_grouptipitem.h"

GroupTipItem::GroupTipItem(QWidget *parent) :
    ListItemBase(parent), _tip(""),
    ui(new Ui::GroupTipItem)
{
    ui->setupUi(this);
    this->setItemType(ListItemType::GROUP_TIP_ITEM);    // 设置item类别
}

GroupTipItem::~GroupTipItem()
{
    delete ui;
}

QSize GroupTipItem::sizeHint()
{
    return QSize(250, 25);
}

void GroupTipItem::setGroupTip(QString str)
{
    ui->label->setText(str);
}
