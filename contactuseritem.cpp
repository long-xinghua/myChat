#include "contactuseritem.h"
#include "ui_contactuseritem.h"

ContactUserItem::ContactUserItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ContactUserItem)
{
    ui->setupUi(this);
    // 设置item类型
    this->setItemType(ListItemType::CONTACT_USER_ITEM);
    // 让红点置于最上层
    ui->redPoint->raise();
    this->showRedPoint(false);  // 默认不显示小红点
}

ContactUserItem::~ContactUserItem()
{
    delete ui;
}

QSize ContactUserItem::sizeHint()
{
    return QSize(250, 70);
}

void ContactUserItem::setInfo(std::shared_ptr<AuthInfo> authInfo)
{
    // 获取_userInfo
    _userInfo = std::make_shared<UserInfo>(authInfo);
    // 设置item的图标
    QPixmap pixmap(_userInfo->_icon);
    ui->iconLabel->setPixmap(pixmap.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->iconLabel->setScaledContents(true);
    // 设置item的文本
    ui->userNameLabel->setText(_userInfo->_name);
}

void ContactUserItem::setInfo(std::shared_ptr<AuthRsp> authResp)
{
    // 获取_userInfo
    _userInfo = std::make_shared<UserInfo>(authResp);
    // 设置item的图标
    QPixmap pixmap(_userInfo->_icon);
    ui->iconLabel->setPixmap(pixmap.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->iconLabel->setScaledContents(true);
    // 设置item的文本
    ui->userNameLabel->setText(_userInfo->_name);
}

void ContactUserItem::setInfo(int uid, QString name, QString icon)
{
    // 获取_userInfo
    _userInfo = std::make_shared<UserInfo>(uid, name, icon);
    // 设置item的图标
    QPixmap pixmap(_userInfo->_icon);
    ui->iconLabel->setPixmap(pixmap.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->iconLabel->setScaledContents(true);
    // 设置item的文本
    ui->userNameLabel->setText(_userInfo->_name);
}

std::shared_ptr<UserInfo> ContactUserItem::getInfo()
{
    return _userInfo;
}

void ContactUserItem::showRedPoint(bool show)
{
    if(show){
        ui->redPoint->show();
    }
    else{
        ui->redPoint->hide();
    }
}
