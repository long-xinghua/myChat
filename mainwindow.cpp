#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 将登录对话框添加到主窗口中心控件位置
    loginDlg = new LoginDialog(this);   //  对话框比主窗口大会导致嵌不进去
    setCentralWidget(loginDlg);
    //loginDlg->show();
    // 添加注册界面
    registerDlg = new RegisterDialog(this);
    // 点击注册按钮跳转到注册页面
    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::switch2reg_slot);

    loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);  // 把对话框设置成无边框的样式
    registerDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    registerDlg->hide();
}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::switch2reg_slot(){
    setCentralWidget(registerDlg);
    loginDlg->hide();
    registerDlg->show();
}
