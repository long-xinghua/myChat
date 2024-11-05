#ifndef HTTPMGR_H
#define HTTPMGR_H
#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

/***********************************************
    * @file     httpmgr.h
    * @brief    http manager单例类
    * 
    * @author   wmlatiao
    * @date     2024/09/27
    * @history
***********************************************/

// 继承Singleton单例模板类，模板变量就是HttpMgr本身，CRTP
class HttpMgr: public QObject,public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT    //写这个宏才能用Qt的高级特性
public:
    ~HttpMgr();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod); // 发送http的函数
private:
    friend class Singleton<HttpMgr>;    // Singleton的_instance会用new来构造出一个HttpMgr对象，如果不把基类作为友元则调用不了HttpMgr的构造函数
    HttpMgr();
    QNetworkAccessManager _manager; // Qt的网络管理者

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);  //槽函数的参数要和信号参数匹配
signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod); //一个http发送完后发送这个信号
    void sig_reg_mod_finish(ReqId id,QString res,ErrorCodes err);    // 发送给注册模块的“http发送完成”信号
    void sig_reset_mod_finish(ReqId id,QString res,ErrorCodes err); // 发送给重置密码模块的“http发送完成”信号
    void sig_login_mod_finish(ReqId id,QString res,ErrorCodes err); // 发送给登录模块的“http发送完成”信号
};

#endif // HTTPMGR_H
