#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent) :
    ListItemBase(parent), _added(false),
    ui(new Ui::ApplyFriendItem)
{
    ui->setupUi(this);
    this->setItemType(ListItemType::APPLY_FRIEND_ITEM);

    ui->addButton->setState("normal", "hover", "press");
    ui->addButton->hide();
    // 点击同意后触发sig_auth_friend信号，在applyFriendPage里处理
    connect(ui->addButton, &ClickedBtn::clicked, [this](){
        emit this->sig_auth_friend(_applyInfo);
    });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::setInfo(std::shared_ptr<ApplyInfo> applyInfo)
{
    _applyInfo = applyInfo;
    QPixmap pixmap(_applyInfo->_iconPath);

    ui->iconLabel->setPixmap(pixmap.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->iconLabel->setScaledContents(true);

    ui->userNameLabel->setText(_applyInfo->_name);
    ui->userChatLabel->setText(_applyInfo->_desc);
}

void ApplyFriendItem::showAddBtn(bool show)
{
    if(show){
        ui->doneLabel->hide();
        ui->addButton->show();
        refresh(this);
        _added = false;
    }else{
        ui->doneLabel->show();
        ui->addButton->hide();
        refresh(this);
        _added = true;
    }
}

QSize ApplyFriendItem::sizeHint() const
{
    return QSize(250, 80);
}

int ApplyFriendItem::getUid()
{
    // 如果_applyInfo还没有值，返回-1
    if(!_applyInfo){
        return -1;
    }
    return _applyInfo->_uid;
}
