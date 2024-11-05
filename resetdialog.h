#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"

/***********************************************
    * @file     resetdialog.h
    * @brief    重置密码界面
    *
    * @author   wmlatiao
    * @date     2024/11/01
    * @history
***********************************************/

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = 0);
    ~ResetDialog();
    
signals:
    void switch2Login();    // 返回登陆界面

private slots:
    
    void on_verifyButton_clicked(); // 点击“获取”按钮槽函数   
    void on_sureButton_clicked();   // 点击“确认”按钮槽函数  
    void on_returnButton_clicked(); // 点击“返回”按钮槽函数
    
    void slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err);  // 网络请求完成的槽函数
    
private:
    Ui::ResetDialog *ui;
    QMap<TipErr, QString> _tip_errs;    // 储存错误标签要显示的错误信息
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers; // 保存回调函数的容器
    
    bool checkUserValid();  
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVarifyValid();
    
    void AddTipErr(TipErr te, QString tips);    // 用来往错误列表里添加错误
    void DelTipErr(TipErr te);                  // 删除错误列表中的错误
    void showTip(QString str, bool ok); // 用于在错误标签上显示信息
    void initHttpHandlers();
    

    
};

#endif // RESETDIALOG_H
