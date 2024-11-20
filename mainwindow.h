#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "chatdialog.h"

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
    ChatDialog* chatDialog;         // 聊天界面

public slots:
    void slotSwitch2reg(); // 切换到注册页面
    void slotSwitch2Login();   //切换到登陆界面
    void slotSwitch2reset();   //切换到重置界面
    // void slotSiwtch2Login();  //从重置界面切换回登陆界面（有必要吗）
    void slotSwitch2chat();   // 切换到聊天界面
};

#endif // MAINWINDOW_H
