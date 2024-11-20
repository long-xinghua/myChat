#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmgr.h"

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
    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::slotSwitch2reg);
    
    // 点击忘记密码按钮跳转到重置页面
    connect(loginDlg, &LoginDialog::switch2reset, this, &MainWindow::slotSwitch2reset);

    // 登陆成功跳转到聊天界面
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_switch_chatDlg, this, &MainWindow::slotSwitch2chat);

    // 测试用，直接跳转到聊天界面
    //emit TcpMgr::GetInstance()->sig_switch_chatDlg();
}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::slotSwitch2reg(){
    // 添加注册界面
    registerDlg = new RegisterDialog(this);

   
    registerDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    //连接注册界面返回登录信号
    connect(registerDlg, &RegisterDialog::sigReturnToLogin, this, &MainWindow::slotSwitch2Login);
    
    // 切换到注册界面后登陆界面就被析构了，之后返回登录界面的话要重新创建对象

    setCentralWidget(registerDlg);
//    loginDlg->hide();
//    registerDlg->show();
}

void MainWindow::slotSwitch2Login()
{
    // 这里需要重新创建loginDialog？？
    loginDlg = new LoginDialog(this);   //  对话框比主窗口大会导致嵌不进去
    loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);  // 把对话框设置成无边框的样式

    // 切换到登陆界面后注册界面就被析构了
    setCentralWidget(loginDlg);
    //loginDlg->show();

    // 点击注册按钮跳转到注册页面
    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::slotSwitch2reg);
    // 点击忘记密码按钮跳转到重置页面
    connect(loginDlg, &LoginDialog::switch2reset, this, &MainWindow::slotSwitch2reset);
    
}

void MainWindow::slotSwitch2reset()
{
    qDebug()<<"switch2reset_slot函数";
    // 添加重置界面
    resetDlg = new ResetDialog(this);
    resetDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    connect(resetDlg, &ResetDialog::switch2Login, this, &MainWindow::slotSwitch2Login);
    //切换到重置界面
    setCentralWidget(resetDlg);
//    loginDlg->hide();
//    resetDlg->show();
    
}

//void MainWindow::slotSiwtch2Login()
//{
//    // 这里需要重新创建loginDialog？？
//    loginDlg = new LoginDialog(this);   //  对话框比主窗口大会导致嵌不进去
//    loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);  // 把对话框设置成无边框的样式

//    // 切换到登陆界面后重置界面就被析构了
//    setCentralWidget(loginDlg);
//    //resetDlg->hide();
//    //loginDlg->show();

//    // 点击注册按钮跳转到注册页面
//    connect(loginDlg, &LoginDialog::switch2reg, this, &MainWindow::slotSwitch2reg);
//    // 点击忘记密码按钮跳转到重置页面
//    //    connect(loginDlg, &LoginDialog::switch2reset, this, &MainWindow::switch2reset_slot);
//}

void MainWindow::slotSwitch2chat()
{
    chatDialog = new ChatDialog(this);
    chatDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    // 切换到聊天界面
    setCentralWidget(chatDialog);
//    chatDialog->show();
//    loginDlg->hide();

    this->setMinimumSize(QSize(600, 400));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
