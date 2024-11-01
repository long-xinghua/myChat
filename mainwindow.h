#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
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
    LoginDialog* loginDlg;
    RegisterDialog* registerDlg;

public slots:
    void switch2reg_slot(); // 切换到注册页面
    void switch2Login_slot();   //切换到登陆界面
};

#endif // MAINWINDOW_H
