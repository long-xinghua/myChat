#include "chatuserwid.h"
#include "ui_chatuserwid.h"
#include <QPixmap>

ChatUserWid::ChatUserWid(QWidget *parent) : ListItemBase(parent),
    ui(new Ui::ChatUserWid)
{
    ui->setupUi(this);
    this->setItemType(ListItemType::CHAT_USER_ITEM);    // 默认控件类型为聊天用户条目
}

ChatUserWid::~ChatUserWid()
{
    delete ui;
}

QSize ChatUserWid::sizeHint() const
{
    return QSize(250, 70);
}

void ChatUserWid::setInfo(QString name, QString head, QString msg)
{
    _name = name;
    _head = head;
    _msg = msg;
    // 加载用户头像
    QPixmap pixmap(_head);
    // 设置图像自动缩放，把图像缩放到iconLabel的标签大小
    ui->iconLabel->setPixmap(pixmap.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->iconLabel->setScaledContents(true); // 缩放label里的内容

    ui->nameLabel->setText(_name);
    ui->msgLabel->setText(_msg);

}
