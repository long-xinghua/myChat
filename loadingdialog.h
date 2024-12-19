#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>

/***********************************************
    * @file     loadingdialog.h
    * @brief    显示加载图标的对话框，表示正在加载
    *
    * @author   wmlatiao
    * @date     2024/11/22
    * @history
***********************************************/

namespace Ui {
class LoadingDialog;
}

class LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();

private:
    Ui::LoadingDialog *ui;
};

#endif // LOADINGDIALOG_H
