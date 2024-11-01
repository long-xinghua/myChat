#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 将登录对话框添加到主窗口中心控件位置
    loginDlg = new LoginDialog(this);   //  对话框比主窗口大会导致嵌不进去
    loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);  // 把对话框设置成无边框的样式
    setCentralWidget(loginDlg);
    //loginDlg->show();
    
    // 点击注册按钮跳转到注册页面
    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::switch2reg_slot);
    
    
}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::switch2reg_slot(){
    // 添加注册界面
    registerDlg = new RegisterDialog(this);

   
    registerDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    //连接注册界面返回登录信号
    connect(registerDlg, &RegisterDialog::sigReturnToLogin, this, &MainWindow::switch2Login_slot);
    
    // 切换到注册界面后登陆界面就被析构了，之后返回登录界面的话要重新创建对象

    setCentralWidget(registerDlg);
//    loginDlg->hide();
//    registerDlg->show();
}

void MainWindow::switch2Login_slot()
{
    // 这里需要重新创建loginDialog？？
    loginDlg = new LoginDialog(this);   //  对话框比主窗口大会导致嵌不进去
    loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);  // 把对话框设置成无边框的样式

    // 切换到登陆界面后注册界面就被析构了
    setCentralWidget(loginDlg);
    //loginDlg->show();

    // 点击注册按钮跳转到注册页面
    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::switch2reg_slot);

}
