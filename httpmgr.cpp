#include "httpmgr.h"

HttpMgr::~HttpMgr()
{
    
}

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish); //连接http发送结束的信号和槽
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson(); //将json数据转化成QByteArray字节序列，用于tcp传输
    QNetworkRequest request(url);   // 创建网络请求对象，构造时指定目标url
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");  //设置请求头，指明发送的数据类型为JSON
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length())); //设置请求头，指明发送数据的长度，以确保接收方知道要读取多少字节。
    auto self = shared_from_this(); //使用 shared_from_this()获取指向当前对象的智能指针，以便在 Lambda 中保持对当前对象的共享指针
    QNetworkReply* reply = _manager.post(request, data);    //使用网络管理器_manager发送POST请求，返回reply用于处理响应，后面要手动回收reply,reply并不一定能马上收到

    connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){
        //处理错误情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug()<<reply->errorString();
            //发送信号通知完成
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();   //告诉Qt，reply不用的时候回收一下
            return;
        }
        //无错误
        QString res = reply->readAll();
        //发送信号通知完成
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD){
        // 发送信号通知指定模块，http响应结束了
        emit sig_reg_mod_finish(id,res,err);    //注册模块的http响应完成的信号，接下来交给RegisterDialog中对应的槽函数处理
    }
}


