#include "usermgr.h"

UserMgr::UserMgr()
{

}

UserMgr::~UserMgr()
{

}

void UserMgr::setName(QString name)
{
    _name = name;
}

void UserMgr::setUid(int uid)
{
    _uid = uid;
}

void UserMgr::setToken(QString token)
{
    _token = token;
}

void UserMgr::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    _userInfo = userInfo;
}

std::vector<std::shared_ptr<ApplyInfo> > UserMgr::getApplyList()
{
    return _applyList;
}

bool UserMgr::alreadyApply(int uid)
{
    for(auto& apply : _applyList){
        if(apply->_uid == uid){
            return true;
        }
    }
    return false;
}

void UserMgr::addApplyList(std::shared_ptr<ApplyInfo> apply)
{
    _applyList.push_back(apply);
}

void UserMgr::appendApplyList(QJsonArray array)
{
    for(const QJsonValue &value : array){
        QJsonObject obj = value.toObject();
        auto name = obj["name"].toString();
        auto uid = obj["uid"].toInt();
        auto status = obj["status"].toInt();
        auto sex = obj["sex"].toInt();
        auto desc = obj["desc"].toString();
        auto nick = obj["nick"].toString();
        auto icon = obj["icon"].toString();

        auto info = std::make_shared<ApplyInfo>(uid, name, desc, icon, nick, sex, status);
        _applyList.push_back(info);
    }
}

QString UserMgr::getName()
{
    return _name;
}

int UserMgr::getUid()
{
    return _uid;
}


