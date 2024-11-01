#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"
#include <QMap>
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
    void on_getCode_clicked();  // 会自动与“获取”按钮的点击事件连接的槽函数,在ui里点转到槽即可获取（on代表这是槽函数，getCode是按钮对象，clicked是事件）
    void slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err);  // 从httpmgr传过来的sig_reg_mod_finish信号对应的槽函数

    void on_comfirmButton_clicked();    // 点击“确定”按钮的槽函数，实现注册功能

    void on_returnButton_clicked();
    
private:
    void initHttpHndlers(); //用于注册处理不同事件的回调函数
    Ui::RegisterDialog *ui;
    void showTip(QString str, bool ok); // 用于在错误标签上显示信息
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;    // 用map绑定请求id和相应的响应函数对象，就可以使用如_handlers[id]()来调用相应函数
    void changeTipPage();

    void AddTipErr(TipErr te, QString tips);    // 用来往错误列表里添加错误
    void DelTipErr(TipErr te);                  // 删除错误列表中的错误
    QMap<TipErr, QString> _tip_errs;

    bool checkUserValid();  //
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();

    QTimer* _countdownTimer;    // 设置定时器
    int _countdown;             // 设置自动返回主界面的倒计时时间

signals:
    void sigReturnToLogin();


};

#endif // REGISTERDIALOG_H
