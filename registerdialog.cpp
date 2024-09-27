#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->passwdEdit->setEchoMode(QLineEdit::Password);   // 输入模式切换成密码模式，在输入时就看不到输入内容了
    ui->confirmEdit->setEchoMode(QLineEdit::Password);
    ui->errLabel->setProperty("state","normal"); // 给错误标签设置属性，把"state"属性设置成"normal"
    ui->errLabel->setText(" ");
    refresh(ui->errLabel);  // 刷新一下标签
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_getCode_clicked()
{
    auto email = ui->mailEdit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");   //用于匹配邮箱格式的正则表达式
    bool match = regex.match(email).hasMatch();
    if(match){
        // 匹配成功，发送http验证码
        showTip(tr("验证码已发送"),true);
    }else{
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::showTip(QString str, bool ok)
{
    ui->errLabel->setText(str); // 在错误标签上显示信息
    if(ok){ // 根据信息是否正确来更改标签qss样式
        ui->errLabel->setProperty("state","normal");
    }else{
        ui->errLabel->setProperty("state","err");
    }
    refresh(ui->errLabel);  // 刷新
}
