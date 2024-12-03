#include "adduseritem.h"
#include "ui_adduseritem.h"

AddUserItem::AddUserItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::AddUserItem)
{
    ui->setupUi(this);
    setItemType(ListItemType::ADD_USER_TIP_ITEM);   // 将本物品类型设置为添加用户提示类型
}

AddUserItem::~AddUserItem()
{
    delete ui;
}

QSize AddUserItem::sizeHint() const
{
     return QSize(250, 70);
}
