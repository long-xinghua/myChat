#include "usermgr.h"

UserMgr::UserMgr():_userInfo(nullptr), _chat_loaded(0), _contact_loaded(0)
{

}

UserMgr::~UserMgr()
{

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

void UserMgr::appendFriendList(QJsonArray array)
{
    for(const QJsonValue &value : array){
        QJsonObject obj = value.toObject();
        auto name = obj["name"].toString();
        auto uid = obj["uid"].toInt();
        auto sex = obj["sex"].toInt();
        auto desc = obj["desc"].toString();
        auto nick = obj["nick"].toString();
        auto icon = obj["icon"].toString();
        auto back = obj["back"].toString();

        auto info = std::make_shared<FriendInfo>(uid, name, nick, icon, sex, desc, back);
        _friendMap[uid] = info;
        _friendList.push_back(info);
    }
}

QString UserMgr::getName()
{
    return _userInfo->_name;
}

int UserMgr::getUid()
{
    return _userInfo->_uid;
}

QString UserMgr::getIcon()
{
    return _userInfo->_icon;
}

bool UserMgr::checkFriendByUid(int uid)
{
    auto iter = _friendMap.find(uid);
    if(iter == _friendMap.end()){
        return false;
    }
    return true;
}

void UserMgr::addFriend(std::shared_ptr<AuthInfo> authInfo)
{
    auto friendInfo = std::make_shared<FriendInfo>(authInfo);
    _friendMap.insert(friendInfo->_uid, friendInfo);
    _friendList.push_back(friendInfo);
}

void UserMgr::addFriend(std::shared_ptr<AuthRsp> authRsp)
{
    auto friendInfo = std::make_shared<FriendInfo>(authRsp);
    _friendMap.insert(friendInfo->_uid, friendInfo);
    _friendList.push_back(friendInfo);
}

std::shared_ptr<FriendInfo> UserMgr::getFriendByUid(int uid)
{
    auto find_iter = _friendMap.find(uid);
    if(find_iter == _friendMap.end()){
        return nullptr;
    }
    return *find_iter;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::getChatListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
    int begin = _chat_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;
    // 起始位置已经超过了_friendList的大小
    if (begin >= _friendList.size()) {
        return friend_list;
    }
    // 结束位置超过了_friendList的大小，全部加载完
    if (end > _friendList.size()) {
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friendList.begin() + begin, _friendList.end());
        return friend_list;
    }
    // 从_friendList的begin() + begin加载到begin()+ end
    friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friendList.begin() + begin, _friendList.begin()+ end);
    return friend_list;
}

bool UserMgr::isLoadChatFin()
{
    if (_chat_loaded >= _friendList.size()) {
        return true;
    }
    return false;
}

void UserMgr::updateChatLoadedCount()
{
    // 更新前的状态
    int begin = _chat_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;
    // 更新后的状态
    if (begin >= _friendList.size()) {
        return ;
    }

    if (end > _friendList.size()) {
        _chat_loaded =_friendList.size();
        return ;
    }

    _chat_loaded = end;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::getConListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
    int begin = _contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;
    // 起始位置已经超过了_friendList的大小
    if (begin >= _friendList.size()) {
        return friend_list;
    }
    // 结束位置超过了_friendList的大小，全部加载完
    if (end > _friendList.size()) {
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friendList.begin() + begin, _friendList.end());
        return friend_list;
    }
    // 从_friendList的begin() + begin加载到begin()+ end
    friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friendList.begin() + begin, _friendList.begin()+ end);
    return friend_list;
}

void UserMgr::updateContactLoadedCount()
{
    // 更新前的状态
    int begin = _contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;
    // 更新后的状态
    if (begin >= _friendList.size()) {
        return ;
    }

    if (end > _friendList.size()) {
        _contact_loaded =_friendList.size();
        return ;
    }

    _contact_loaded = end;
}

bool UserMgr::isLoadConFin()
{
    if (_contact_loaded >= _friendList.size()) {
        return true;
    }
    return false;
}


