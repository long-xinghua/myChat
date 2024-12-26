#include "tcpmgr.h"
#include <QAbstractSocket>  // 要用到socket一些具象化处理就要用到这个类
#include <QJsonDocument>
#include "usermgr.h"

TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr():_host(""), _port(0), _b_recv_pending(false), _message_id(0), _message_len(0)
{
    // _socket根据自身情况会自动地发送各种信号，只需捕获这些信号进行相应处理即可
    // sokect连接成功信号，发现连接成功就发送逻辑层的连接成功信号
    connect(&_socket, &QTcpSocket::connected, [&](){
        qDebug()<<"TCP连接成功";
        // 发连接成功的信号，之后就能发消息通信了
        emit sig_con_success(true);
    });
    
    // 当服务端有消息发送过来时就会触发readyRead信号
    connect(&_socket, &QTcpSocket::readyRead, [&](){
        // 当有数据可读时读取所有数据并放入缓冲区
        _buffer.append(_socket.readAll());
        
        // 用QDatastream对象来管理buffer中的数据
        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);
        
        forever{
            // 如果上一个消息收全了就读取数据头，更新_message_id和_message_len，否则用上一次的就行
            if(!_b_recv_pending){
                // 检查数据流的数据是否够解析出消息头（4字节,两字节的id和两字节的消息长度）
                if(_buffer.size() < sizeof(quint16)*2){
                    return; // 直接返回，等待下一次缓冲区可读
                }
                // 解析头部，包含数据包id和内容长度
                stream>>_message_id>>_message_len;
                qDebug()<<"message id: "<<_message_id<<"    message length: "<<_message_len;
                //移除前四个字节
                _buffer = _buffer.mid(sizeof(quint16)*2);   // 第2个参数默认为-1，提取从sizeof(quint16)*2+1到末尾的所有字节
            }

            // 检查buffer长度是否满足_message_len记录的长度，不满足则等下一次可读
            if(_buffer.size() < _message_len){
                _b_recv_pending = true; // 消息没收全，_b_recv_pending置为true
                return;
            }
            // 读取消息
            _b_recv_pending = false;
            QByteArray messageBody = _buffer.mid(0, _message_len);  // 从第一个字节开始截取，截取_message_len长度的字节数
            qDebug()<<"收到的消息是："<<messageBody;
            _buffer = _buffer.mid(_message_len);
            handleMsg(ReqId(_message_id), _message_len, messageBody); // 交给handleMsg处理数据,handleMsg中会调用_handlers中的回调函数
        }
    });


    //5.15 之后版本
//       QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
//           Q_UNUSED(socketError)
//           qDebug() << "Error:" << _socket.errorString();
//       });

    // 处理错误（适用于Qt 5.15之前的版本）,捕获socket里的错误信号
    QObject::connect(&_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                        [&](QTcpSocket::SocketError socketError) {
           qDebug() << "Error:" << _socket.errorString() ;
           // 不管触发了什么错误都是发起sig_con_success(false)信号，代表连接失败
           switch (socketError) {
               case QTcpSocket::ConnectionRefusedError:
                   qDebug() << "Connection Refused!";
                   emit sig_con_success(false);
                   break;
               case QTcpSocket::RemoteHostClosedError:
                   qDebug() << "Remote Host Closed Connection!";
                   break;
               case QTcpSocket::HostNotFoundError:
                   qDebug() << "Host Not Found!";
                   emit sig_con_success(false);
                   break;
               case QTcpSocket::SocketTimeoutError:
                   qDebug() << "Connection Timeout!";
                   emit sig_con_success(false);
                   break;
               case QTcpSocket::NetworkError:
                   qDebug() << "Network Error!";
                   break;
               default:
                   qDebug() << "Other Error!";
                   break;
           }
     });

    // 处理连接断开
    connect(&_socket, &QTcpSocket::disconnected, [&](){
        qDebug()<<"Disconnected from Tcp server";
    });
    // 连接发送信号来发送数据(要发送消息时通过访问TcpMgr的单例触发sig_send_data的信号就行了)
    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    //注册回调函数
    initHandlers();
    
}

void TcpMgr::initHandlers()
{
    // 收到登录请求的ChatServer服务器回包时调用这个回调
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);  // 告诉Qt我不会使用len
        qDebug()<<"request id is: "<<id<<", data is: "<<data;
        // 将QByteArray转为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查是否解析成功
        if(jsonDoc.isNull() || !jsonDoc.isObject()){
            qDebug()<<"json解析失败";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            qDebug()<<"json中没有error字段";
            int err = ErrorCodes::ERR_JSON;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug()<<"登陆失败，错误代码为："<<err;
            emit sig_login_failed(err);
            return;
        }

        // 先保存一下用户信息
        auto uid = jsonObj["uid"].toInt();
        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto userInfo = std::make_shared<UserInfo>(uid, name, nick, icon, sex);
        UserMgr::GetInstance()->setUserInfo(userInfo);
        UserMgr::GetInstance()->setToken(jsonObj["token"].toString());
        // 保存用户收到的好友申请的信息
        if(jsonObj.contains("apply_list")){
            UserMgr::GetInstance()->appendApplyList(jsonObj["apply_list"].toArray());   // 转为json数组传进去
        }

        // 保存用户收到的好友列表的信息
        if(jsonObj.contains("friend_list")){
            UserMgr::GetInstance()->appendFriendList(jsonObj["friend_list"].toArray());
        }
        // 发送切换聊天界面的信号
        emit sig_switch_chatDlg();
        qDebug()<<"跳转到登陆界面";
    });

    // 查找用户请求回包的回调
    _handlers.insert(ReqId::ID_SEARCH_USER_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);  // 告诉Qt我不会使用len
        qDebug()<<"request id is: "<<id<<", data is: "<<data;
        // 将QByteArray转为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查是否解析成功
        if(jsonDoc.isNull() || !jsonDoc.isObject()){
            qDebug()<<"json解析失败";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            qDebug()<<"json中没有error字段";
            int err = ErrorCodes::ERR_JSON;
            emit sig_user_search(nullptr);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug()<<"查找用户失败，错误代码为："<<err;
            emit sig_user_search(nullptr);
            return;
        }

        std::shared_ptr<SearchInfo> searchInfo = std::make_shared<SearchInfo>(jsonObj["uid"].toInt(), jsonObj["name"].toString(), jsonObj["nick"].toString(),
                                                                              jsonObj["desc"].toString(), jsonObj["sex"].toInt(), jsonObj["icon"].toString());
        // 发送信号提醒搜索已完成
        emit sig_user_search(searchInfo);
    });

    // 添加好友请求回包的回调
    _handlers.insert(ReqId::ID_ADD_FRIEND_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);  // 告诉Qt我不会使用len
        qDebug()<<"request id is: "<<id<<", data is: "<<data;
        // 将QByteArray转为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查是否解析成功
        if(jsonDoc.isNull() || !jsonDoc.isObject()){
            qDebug()<<"json解析失败";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            qDebug()<<"json中没有error字段";
            int err = ErrorCodes::ERR_JSON;
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug()<<"查找用户失败，错误代码为："<<err;
            return;
        }

        // 申请好友的请求发送成功
        qDebug()<<"好友申请发送成功";
    });

    // 收到好友申请提示
    _handlers.insert(ReqId::ID_NOTIFY_ADD_FRIEND_REQ, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);  // 告诉Qt我不会使用len
        qDebug()<<"request id is: "<<id<<", data is: "<<data;
        // 将QByteArray转为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查是否解析成功
        if(jsonDoc.isNull() || !jsonDoc.isObject()){
            qDebug()<<"json解析失败";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            qDebug()<<"json中没有error字段";
            int err = ErrorCodes::ERR_JSON;
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug()<<"新好友申请提示出错，错误代码为："<<err;
            return;
        }

        // 取出data中的信息
        int from_uid = jsonObj["from_uid"].toInt();
        QString applyName = jsonObj["applyName"].toString();
        QString desc = jsonObj["desc"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();
        QString icon = jsonObj["icon"].toString();

        auto apply_info = std::make_shared<AddFriendApply>(from_uid, applyName, desc, icon, nick, sex);
        emit sig_firend_apply(apply_info);  // 发送收到好友申请的信号
        qDebug()<<"收到一条好友申请";
    });

    // 收到新好友认证成功提醒的回调
    _handlers.insert(ReqId::ID_NOTIFY_AUTH_FRIEND_REQ, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);  // 告诉Qt我不会使用len
        qDebug()<<"request id is: "<<id<<", data is: "<<data;
        // 将QByteArray转为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查是否解析成功
        if(jsonDoc.isNull() || !jsonDoc.isObject()){
            qDebug()<<"json解析失败";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            qDebug()<<"json中没有error字段";
            int err = ErrorCodes::ERR_JSON;
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug()<<"新好友认证出错，错误代码为："<<err;
            return;
        }

        // 取出data中的信息
        int from_uid = jsonObj["from_uid"].toInt();
        QString name = jsonObj["name"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();
        QString icon = jsonObj["icon"].toString();

        auto auth_info = std::make_shared<AuthInfo>(from_uid, name, nick, icon, sex);
        emit sig_add_auth_friend(auth_info);  // 发送收到好友认证通过，添加好友的信号
        qDebug()<<"好友认证通过";
    });

    // 认证好友后服务器回包的回调
    _handlers.insert(ReqId::ID_AUTH_FRIEND_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);  // 告诉Qt我不会使用len
        qDebug()<<"request id is: "<<id<<", data is: "<<data;
        // 将QByteArray转为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查是否解析成功
        if(jsonDoc.isNull() || !jsonDoc.isObject()){
            qDebug()<<"json解析失败";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            qDebug()<<"json中没有error字段";
            int err = ErrorCodes::ERR_JSON;
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug()<<"新好友认证出错，错误代码为："<<err;
            return;
        }

        // 取出data中对方的信息
        int uid = jsonObj["uid"].toInt();
        QString name = jsonObj["name"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();
        QString icon = jsonObj["icon"].toString();

        auto auth_rsp = std::make_shared<AuthRsp>(uid, name, nick, icon, sex);
        emit sig_auth_rsp(auth_rsp);  // 发送好友认证完成，添加好友的信号
        qDebug()<<"完成好友认证";
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    if(_handlers.find(id) == _handlers.end()){
        qDebug()<<"没有请求id为"<<id<<"对应的回调函数";
        return;
    }
    // 根据id调用相应的回调函数
    _handlers[id](id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug()<<"尝试进行tcp连接,地址："<<si.host<<":"<<si.port;
    _host = si.host;
    _port = static_cast<quint16>(si.port.toInt());
    _socket.connectToHost(_host, _port);    // 连接到服务器，连接过程是异步的，连接情况通过socket发出信号（connected或errorOccurred）来传递
}

void TcpMgr::slot_send_data(ReqId reqId, QByteArray data)
{
    uint16_t id = reqId;
    quint16 len = static_cast<quint16>(data.size());

    // 创建一个字节流block来储存要发送的内容
    QByteArray block;
    // 使用QDataStream来向block中写入数据
    QDataStream out(&block, QIODevice::WriteOnly);
    // 将out设置为网络字节序（大端序）
    out.setByteOrder(QDataStream::BigEndian);

    //先按作者的来看看吧
    out<<id<<len;
    block.append(data);
    // 向服务器发送数据
    _socket.write(block);

}
