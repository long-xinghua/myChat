#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
/***********************************************
    * @file     mainwindow.h
    * @brief    主窗口
    *
    * @author   wmlatiao
    * @date     2024/09/24
    * @history
***********************************************/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginDialog* loginDlg;          // 登录界面
    RegisterDialog* registerDlg;    // 注册界面
    ResetDialog* resetDlg;          // 重置密码界面

public slots:
    void switch2reg_slot(); // 切换到注册页面
    void switch2Login_slot();   //切换到登陆界面
    void switch2reset_slot();   //切换到重置界面
    void siwtch2Login_slot2();  //从重置界面切换回登陆界面（有必要吗）
};

#endif // MAINWINDOW_H
