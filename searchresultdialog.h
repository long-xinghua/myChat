#ifndef SEARCHRESULTDIALOG_H
#define SEARCHRESULTDIALOG_H

#include <QDialog>
#include <memory>
#include "userdata.h"

/***********************************************
    * @file     searchresultdialog.h
    * @brief    点击搜索条目后跳出来的搜索结果对话框
    *
    * @author   wmlatiao
    * @date     2024/12/04
    * @history
***********************************************/

namespace Ui {
class searchResultDialog;
}

class searchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchResultDialog(QWidget *parent = 0);
    ~searchResultDialog();

    void setSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    Ui::searchResultDialog *ui;

    std::shared_ptr<SearchInfo> _si;    // 保存用户信息
    QWidget* _parent;                    // 用于给父窗口传递信息

private slots:
    void on_addFriendButton_clicked();
};

#endif // SEARCHRESULTDIALOG_H
