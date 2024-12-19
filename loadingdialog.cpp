#include "loadingdialog.h"
#include "ui_loadingdialog.h"
#include <QMovie>
#include <QDebug>
#include <QFile>

LoadingDialog::LoadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    // 之前没有在下面加上Qt::Dialog导致对话框加载不出来
    this->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);    // 隐藏菜单栏，设置成无边框，对话框始终位于顶层
    this->setAttribute(Qt::WA_TranslucentBackground);   // 设置透明背景
    // 将对话框设成全屏尺寸
    //qDebug()<<"设置加载对话框大小，父窗口大小为："<<parent->size();
    this->setFixedSize(parent->size()); // 设置成父对象的大小


    //qDebug() << "Loading GIF path exists:" << QFile(":res/loading.gif").exists();
    QMovie* movie = new QMovie(":/res/loading.gif");
    ui->loadingLabel->setMovie(movie);  // 把标签内容设置为加载的gif图，此时movie归属于loadingLabel，会跟着一起析构
    movie->start();
}

LoadingDialog::~LoadingDialog()
{
    qDebug()<<"loadingDialog析构";
    delete ui;
}
