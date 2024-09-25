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

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
