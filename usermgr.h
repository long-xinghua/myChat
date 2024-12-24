#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "singleton.h"
#include "userdata.h"
#include <vector>
#include <QJsonArray>



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
    void setName(QString name);
    void setUid(int uid);
    void setToken(QString token);
    void setUserInfo(std::shared_ptr<UserInfo> userInfo);       // 设置本用户信息
    std::vector<std::shared_ptr<ApplyInfo>> getApplyList();     // 获取申请信息列表
    bool alreadyApply(int uid);                                 // 查询是否已有某个申请人的信息
    void addApplyList(std::shared_ptr<ApplyInfo> apply);        // 向申请列表中添加申请信息
    void appendApplyList(QJsonArray array);                     // 通过json数组添加已有的好友申请信息
    QString getName();
    int getUid();
private:
    UserMgr();

    QString _name;
    QString _token;
    int _uid;
    std::vector<std::shared_ptr<ApplyInfo>> _applyList; // 保存该用户收到的好友申请信息
    std::shared_ptr<UserInfo> _userInfo;

signals:

public slots:
};

#endif // USERMGR_H
