#include "searchfaileddialog.h"
#include "ui_searchfaileddialog.h"
#include <QDebug>

SearchFailedDialog::SearchFailedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchFailedDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("查找失败");
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    ui->confirmButton->setState("normal", "hover", "press");
    this->setModal(true);
}

SearchFailedDialog::~SearchFailedDialog()
{
    qDebug()<<"搜索失败对话框析构函数";
    delete ui;
}

void SearchFailedDialog::on_confirmButton_clicked()
{
    this->hide();
}