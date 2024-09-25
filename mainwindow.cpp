#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 将登录对话框添加到主窗口中心控件位置
    loginDlg = new LoginDialog();
    setCentralWidget(loginDlg);
    loginDlg->show();
    // 添加注册界面
    registerDlg = new RegisterDialog();
    // 点击注册按钮跳转到注册页面
    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::switch2reg_slot);
}


MainWindow::~MainWindow()
{
    delete ui;
    //由于登录和注册等界面没设置父节点，需要手动回收资源
    if(loginDlg){
        delete loginDlg;
        loginDlg = nullptr;
    }
    if(registerDlg){
        delete registerDlg;
        registerDlg = nullptr;
    }
}

void MainWindow::switch2reg_slot(){
    setCentralWidget(registerDlg);
    loginDlg->hide();
    registerDlg->show();
}
