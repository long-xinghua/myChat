#ifndef USERDATA_H
#define USERDATA_H
#include <QString>
#include <memory>

/***********************************************
    * @file     userdata.h
    * @brief    管理用户信息
    *
    * @author   wmlatiao
    * @date     2024/12/03
    * @history
***********************************************/


class SearchInfo
{
public:
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex);
    int _uid;
    QString _name;
    QString _nick;
    QString _desc;
    int _sex;
};

// 添加对方为好友记录的信息
class AddFriendApply {
public:
    AddFriendApply(int from_uid, QString name, QString desc);
    int _from_uid;
    QString _name;
    QString _desc;
};

// 收到好友申请时记录对方的信息
struct ApplyInfo {
    ApplyInfo(int uid, QString name, QString desc,
        QString iconPath, QString nick, int sex, int status)
        :_uid(uid),_name(name),_desc(desc),
        _iconPath(iconPath),_nick(nick),_sex(sex),_status(status){}

    void setIcon(QString head){
        _iconPath = head;
    }
    int _uid;
    QString _name;
    QString _desc;          // 一些描述性信息
    QString _iconPath;
    QString _nick;
    int _sex;
    int _status;            // 记录该申请是否被通过了
};

//
struct AuthInfo{
    AuthInfo(int uid, QString name, QString nickname, QString iconPath, int sex):
        _uid(uid), _name(name), _nickname(nickname), _iconPath(iconPath), _sex(sex){}
    int _uid;
    QString _name;
    QString _nickname;
    QString _iconPath; 
    int _sex;
};

struct AuthRsp{
    AuthRsp(int peer_uid, QString peer_name, QString peer_nickname, QString peer_iconPath, int peer_sex):
        _uid(peer_uid), _name(peer_name), _nickname(peer_nickname), _iconPath(peer_iconPath), _sex(peer_sex){}
    int _uid;
    QString _name;
    QString _nickname;
    QString _iconPath;
    int _sex;
};

struct UserInfo{
    UserInfo(int uid, QString name, QString nickname, QString iconPath, int sex):
        _uid(uid), _name(name), _nickname(nickname), _iconPath(iconPath), _sex(sex){}
    UserInfo(std::shared_ptr<AuthInfo> auth):
        _uid(auth->_uid), _name(auth->_name), _nickname(auth->_nickname), _iconPath(auth->_iconPath), _sex(auth->_sex){}
    UserInfo(std::shared_ptr<AuthRsp> auth):
        _uid(auth->_uid), _name(auth->_name), _nickname(auth->_nickname), _iconPath(auth->_iconPath), _sex(auth->_sex){}
    UserInfo(int uid, QString name, QString iconPath):
        _uid(uid), _name(name), _nickname(_name), _iconPath(iconPath), _sex(0){}

    int _uid;
    QString _name;
    QString _nickname;
    QString _iconPath;
    int _sex;
};


#endif // USERDATA_H
