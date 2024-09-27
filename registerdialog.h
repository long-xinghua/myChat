#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"
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
    void slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err);  // 从httpmgr传过来的sig_reg_mod_finish信号对应的槽函数

private:
    void initHttpHndlers(); //用于注册处理不同事件的回调函数
    Ui::RegisterDialog *ui;
    void showTip(QString str, bool ok); // 用于在错误标签上显示信息
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;    // 用map绑定请求id和相应的响应函数对象，就可以使用如_handlers[id]()来调用相应函数
};

#endif // REGISTERDIALOG_H
