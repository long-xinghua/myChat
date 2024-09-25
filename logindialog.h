#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
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

signals:
    void switch2reg();

};

#endif // LOGINDIALOG_H
