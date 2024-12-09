#include "searchresultdialog.h"
#include "ui_searchresultdialog.h"
#include <QDir>
#include <QDebug>
#include "applyfriend.h"

searchResultDialog::searchResultDialog(QWidget *parent) :
    QDialog(parent), _parent(parent),
    ui(new Ui::searchResultDialog)
{
    ui->setupUi(this);

    // 隐藏对话框标题栏
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    // 设置对话框标题
    this->setWindowTitle("查找");
    // 获取程序路径
    QString appPath = QCoreApplication::applicationDirPath();
    // 从服务器获取的用户头像会下载到app目录中的static文件夹中，从这里面获取头像。这里先用于测试
    QString pixPath = QDir::toNativeSeparators(appPath+QDir::separator()+"static"+QDir::separator()+"head_1.jpg");
    QPixmap headPix(pixPath);
    headPix = headPix.scaled(ui->headLabel->size(), Qt::KeepAspectRatio);
    ui->headLabel->setPixmap(headPix);
    ui->addFriendButton->setState("normal", "hover", "press");

    // 对话框设置为模态
    this->setModal(true);

}

searchResultDialog::~searchResultDialog()
{
    qDebug()<<"查找结果对话框析构";
    delete ui;
}

void searchResultDialog::setSearchInfo(std::shared_ptr<SearchInfo> si)
{
    _si=si;
    ui->nameLabel->setText(si->_name);
}

void searchResultDialog::on_addFriendButton_clicked()
{
    this->hide();
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->setSearchInfo(_si);
    applyFriend->setModal(true);
    applyFriend->show();
}
