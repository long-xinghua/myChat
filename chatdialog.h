#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

/***********************************************
    * @file     chatdialog.h
    * @brief    聊天界面
    *
    * @author   wmlatiao
    * @date     2024/11/20
    * @history
***********************************************/

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();

private:
    Ui::ChatDialog *ui;
};

#endif // CHATDIALOG_H
