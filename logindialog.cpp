#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 将“点击按钮”信号连接到跳转至“注册页面”信号
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginDialog::switch2reg);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
