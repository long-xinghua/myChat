#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "singleton.h"
#include "userdata.h"
#include <vector>
#include <QJsonArray>
#include <QMap>



/***********************************************
    * @file     usermgr.h
    * @brief    用户管理类，保存登录用户的名字、uid和token
    *
    * @author   wmlatiao
    * @date     2024/11/5
    * @history
***********************************************/

class UserMgr : public QObject, public Singleton<UserMgr>
{
    Q_OBJECT
public:
    //explicit UserMgr(QObject *parent = nullptr);
    friend class Singleton<UserMgr>;

    ~UserMgr();
//    void setUid(int uid);
    void setToken(QString token);
    void setUserInfo(std::shared_ptr<UserInfo> userInfo);       // 设置本用户信息
    std::vector<std::shared_ptr<ApplyInfo>> getApplyList();     // 获取申请信息列表
    bool alreadyApply(int uid);                                 // 查询是否已有某个申请人的信息
    void addApplyList(std::shared_ptr<ApplyInfo> apply);        // 向申请列表中添加申请信息
    void appendApplyList(QJsonArray array);                     // 通过json数组添加已有的好友申请信息
    void appendFriendList(QJsonArray array);                    // 通过json数组添加已有的好友信息
    QString getName();
    int getUid();
    QString getIcon();
    bool checkFriendByUid(int uid);                             // 通过uid查询用户是否已在好友列表中
    void addFriend(std::shared_ptr<AuthInfo> authInfo);         // 重载版本
    void addFriend(std::shared_ptr<AuthRsp> authRsp);
    std::shared_ptr<FriendInfo> getFriendByUid(int uid);        // 通过uid获取好友信息

    std::vector<std::shared_ptr<FriendInfo>> getChatListPerPage();      // 按照每页聊天条目的数量来取出聊天好友
    bool isLoadChatFin();                                               // 判断当前聊天条目是否已加载完成
    void updateChatLoadedCount();                                       // 更新已加载的聊天条目数量
    // 同上，加载的是联系人列表
    std::vector<std::shared_ptr<FriendInfo>> getConListPerPage();
    void updateContactLoadedCount();
    bool isLoadConFin();

private:
    UserMgr();


    QString _token;

    std::vector<std::shared_ptr<ApplyInfo>> _applyList; // 保存该用户收到的好友申请信息
    std::shared_ptr<UserInfo> _userInfo;
    QMap<int, std::shared_ptr<FriendInfo>> _friendMap;                  // 保存用户的好友信息
    std::vector<std::shared_ptr<FriendInfo>> _friendList;               // 保存用户的好友信息
    int _chat_loaded;                                                   // 已经加载了的聊天条目数量
    int _contact_loaded;                                                // 已经加载了的联系人条目数量

signals:

public slots:
};

#endif // USERMGR_H
