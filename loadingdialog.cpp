#include "loadingdialog.h"
#include "ui_loadingdialog.h"
#include <QMovie>

LoadingDialog::LoadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowSystemMenuHint|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);    // 隐藏菜单栏，设置成无边框，对话框始终位于顶层
    this->setAttribute(Qt::WA_TranslucentBackground);   // 设置透明背景
    // 将对话框设成全屏尺寸
    this->setFixedSize(parent->size()); // 设置成父对象的大小

    QMovie* movie = new QMovie(":/res/loading.gif");
    ui->loadingLabel->setMovie(movie);  // 把标签内容设置为加载的gif图，此时movie归属于loadingLabel，会跟着一起析构
    movie->start();

}

LoadingDialog::~LoadingDialog()
{
    delete ui;
}
