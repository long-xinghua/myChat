#ifndef USERDATA_H
#define USERDATA_H
#include <QString>
#include <memory>
#include <vector>
#include <QJsonArray>
#include <QJsonObject>

/***********************************************
    * @file     userdata.h
    * @brief    管理用户信息
    *
    * @author   wmlatiao
    * @date     2024/12/03
    * @history
***********************************************/


// 存放从服务器搜索得到的用户信息
class SearchInfo
{
public:
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex, QString icon);
    int _uid;
    QString _name;
    QString _nick;
    QString _desc;
    int _sex;
    QString _icon;
};

// 收到好友申请时记录对方的信息
class AddFriendApply {
public:
    AddFriendApply(int from_uid, QString name, QString desc, QString icon, QString nick, int sex);
    int _from_uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    int _sex;
};

//
struct ApplyInfo {
    ApplyInfo(int uid, QString name, QString desc,
        QString icon, QString nick, int sex, int status)
        :_uid(uid),_name(name),_desc(desc),
        _icon(icon),_nick(nick),_sex(sex),_status(status){}

    ApplyInfo(std::shared_ptr<AddFriendApply> addInfo):
        _uid(addInfo->_from_uid),_name(addInfo->_name),_desc(addInfo->_desc),
        _icon(addInfo->_icon),_nick(addInfo->_nick),_sex(addInfo->_sex),_status(0){}

    void setIcon(QString head){
        _icon = head;
    }
    int _uid;
    QString _name;
    QString _desc;          // 一些描述性信息
    QString _icon;
    QString _nick;
    int _sex;
    int _status;            // 记录该申请是否被通过了
};

// 完成好友认证的用户信息
struct AuthInfo{
    AuthInfo(int uid, QString name, QString nick, QString icon, int sex):
        _uid(uid), _name(name), _nick(nick), _icon(icon), _sex(sex){}
    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
};

struct AuthRsp{
    AuthRsp(int peer_uid, QString peer_name, QString peer_nick, QString peer_icon, int peer_sex):
        _uid(peer_uid), _name(peer_name), _nick(peer_nick), _icon(peer_icon), _sex(peer_sex){}
    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
};

struct TextChatData;
// 好友信息
struct FriendInfo{
    FriendInfo(int uid, QString name, QString nick, QString icon,
        int sex, QString desc, QString back, QString last_msg=""):_uid(uid),
        _name(name),_nick(nick),_icon(icon),_sex(sex),_desc(desc),
        _back(back),_last_msg(last_msg){}

    FriendInfo(std::shared_ptr<AuthInfo> auth_info):_uid(auth_info->_uid),
    _nick(auth_info->_nick),_icon(auth_info->_icon),_name(auth_info->_name),
      _sex(auth_info->_sex){}

    FriendInfo(std::shared_ptr<AuthRsp> auth_rsp):_uid(auth_rsp->_uid),
    _nick(auth_rsp->_nick),_icon(auth_rsp->_icon),_name(auth_rsp->_name),
      _sex(auth_rsp->_sex){}
    
    void appendChatMsgs(const std::vector<std::shared_ptr<TextChatData>> text_vec);

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    QString _desc;
    QString _back;
    QString _last_msg;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;      // 记录历史聊天内容
};

struct TextChatData;

struct UserInfo{
    UserInfo(int uid, QString name, QString nick, QString icon, int sex, QString last_msg=""):
        _uid(uid), _name(name), _nick(nick), _icon(icon), _sex(sex), _last_msg(last_msg){}
    UserInfo(int uid, QString name, QString icon, QString last_msg=""):
        _uid(uid), _name(name), _nick(_name), _icon(icon), _sex(0), _last_msg(last_msg){}
    UserInfo(std::shared_ptr<AuthInfo> auth):
        _uid(auth->_uid), _name(auth->_name), _nick(auth->_nick), _icon(auth->_icon), _sex(auth->_sex), _last_msg(""){}
    UserInfo(std::shared_ptr<AuthRsp> auth):
        _uid(auth->_uid), _name(auth->_name), _nick(auth->_nick), _icon(auth->_icon), _sex(auth->_sex), _last_msg(""){}
    UserInfo(std::shared_ptr<SearchInfo> search_info):
            _uid(search_info->_uid),_name(search_info->_name),_nick(search_info->_nick),
        _icon(search_info->_icon),_sex(search_info->_sex),_last_msg(""){}
    UserInfo(std::shared_ptr<FriendInfo> friend_info):
        _uid(friend_info->_uid),_name(friend_info->_name),_nick(friend_info->_nick),
        _icon(friend_info->_icon),_sex(friend_info->_sex),_last_msg(""), _chat_msgs(friend_info->_chat_msgs){}

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    QString _last_msg;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

// 单条文字类型聊天数据
struct TextChatData{
    TextChatData(QString msg_id, QString msg_content, int from_uid, int to_uid):
    _msg_id(msg_id), _msg_content(msg_content), _from_uid(from_uid), _to_uid(to_uid) {}
    QString _msg_id;
    QString _msg_content;
    int _from_uid;
    int _to_uid;
};

// 客户端和服务器之间发送的聊天消息,可能包含多条消息
struct TextChatMsg{
    TextChatMsg(int fromuid, int touid, QJsonArray arrays):
        _from_uid(fromuid),_to_uid(touid){
        for(auto  msg_data : arrays){
            auto msg_obj = msg_data.toObject();
            auto content = msg_obj["content"].toString();
            auto msgid = msg_obj["msgid"].toString();
            auto msg_ptr = std::make_shared<TextChatData>(msgid, content,fromuid, touid);
            _chat_msgs.push_back(msg_ptr);
        }
    }
    int _to_uid;
    int _from_uid;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};



#endif // USERDATA_H
