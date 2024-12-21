#ifndef TCPMGR_H
#define TCPMGR_H
#include "singleton.h"
#include "global.h"
#include <QTcpSocket>   // QTcpSocket是Tcp一个类，需要在.pro文件中加载对应模块（network模块）
#include <functional>
#include <QObject>
#include "userdata.h"

/***********************************************
    * @file     tcpmgr.h
    * @brief    tcp管理者类
    *
    * @author   wmlatiao
    * @date     2024/11/5
    * @history
***********************************************/


// 如果要让类有信号和槽的功能就必须继承QObject类,再继承Singleton类实现单例类
class TcpMgr: public QObject, public Singleton<TcpMgr>, public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT    //写这个宏才能用Qt的高级特性，如信号和槽
public:
    ~TcpMgr();
private:
    friend class Singleton<TcpMgr>;
    
    TcpMgr();
    void initHandlers();    // 注册一些回调函数的处理逻辑
    void handleMsg(ReqId id, int len, QByteArray data); // 处理数据
    
    QTcpSocket _socket; // 
    QString _host;          // 服务器地址
    quint16 _port;          // 服务器端口
    QByteArray _buffer;     // 数据缓存区
    bool _b_recv_pending;   // 为true代表数据没有收全，等全部收到，为false代表收全了
    quint16 _message_id;    // 数据包的头两个字节，表示数据包的请求id
    quint16 _message_len;   // 数据包的第3-4字节，表示本数据包长度，若缓冲区中后面长度不够该长度则将_b_recv_pending置为true
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers; // 储存回调函数
public slots:
    void slot_tcp_connect(ServerInfo);  // 尝试进行tcp连接的槽函数
    void slot_send_data(ReqId reqId, QByteArray data);    // 发送数据成功调用该函数
signals:
    void sig_con_success(bool success);                 // 连接阶段结束触发该信号,true代表连接成功，false代表连接失败
    void sig_send_data(ReqId id, QByteArray data);         // 发送数据时触发该信号
    void sig_switch_chatDlg();                          // 切换到聊天界面
    void sig_login_failed(int);                         // 登陆失败信号（连接成功但登陆失败）, 参数为错误代码
    void sig_user_search(std::shared_ptr<SearchInfo>);  // 搜索用户信号

    // 同意对方好友申请、收到对方好友申请、向对方发送好友申请的三个信号
    void sig_auth_rsp(std::shared_ptr<AuthRsp> rsp);            // 向对方发送好友申请，收到同意的消息
    void sig_firend_apply(std::shared_ptr<AddFriendApply>);     // 收到对方好友申请
    void sig_add_auth_apply(std::shared_ptr<AuthInfo>);         // 同意对方好友申请
};

#endif // TCPMGR_H
