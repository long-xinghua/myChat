#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
/***********************************************
    * @file     registerdialog.h
    * @brief    注册界面
    *
    * @author   wmlatiao
    * @date     2024/09/25
    * @history
***********************************************/

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();

private slots:
    void on_getCode_clicked();  // 会自动与按钮的点击事件连接的槽函数,在ui里点转到槽即可获取（on代表这是槽函数，getCode是按钮对象，clicked是事件）

private:
    Ui::RegisterDialog *ui;
    void showTip(QString str, bool ok); // 用于在错误标签上显示信息
};

#endif // REGISTERDIALOG_H
