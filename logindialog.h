#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"

/***********************************************
    * @file     logindialog.h
    * @brief    登陆界面
    *
    * @author   wmlatiao
    * @date     2024/09/25
    * @history
***********************************************/

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    int _uid;
    QString _token;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;   // 储存应对各种请求的回调函数
    QMap<TipErr, QString> _tip_errs;    // 储存错误标签要显示的错误信息


    void initHeadLabel();       // 初始化登陆界面的图片
    void initHttpHandlers();    // 初始化收到Http回包的回调函数
    void showTip(QString str, bool ok); // 用于在错误标签上显示信息
    void AddTipErr(TipErr te, QString tips);    // 用来往错误列表里添加错误并显示当前错误
    void DelTipErr(TipErr te);                  // 删除错误列表中的错误并显示之前的错误    
    bool checkEmailValid(); // 检查邮箱格式是否有误
    bool checkPassValid();  // 检查密码格式是否有误
    void enableBtn(bool enabled);

signals:
    void switch2reg();      // 切换到注册页面的信号
    void switch2reset();    // 切换到重置密码页面的信号
    void sig_connect_tcp(ServerInfo si);
public slots:
    void slot_forget_passwd();

private slots:
    void on_loginButton_clicked();  // 点击登录按钮槽函数
    void slot_login_mod_finish(ReqId id,QString res,ErrorCodes err);    // http响应完成触发的槽函数
};

#endif // LOGINDIALOG_H
