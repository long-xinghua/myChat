#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "singleton.h"


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

    QString getName();
    int getUid();
private:
    UserMgr();

    QString _name;
    QString _token;
    int _uid;


signals:

public slots:
};

#endif // USERMGR_H
