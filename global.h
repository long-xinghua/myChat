#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include "QStyle"
#include <memory>   // 智能指针
#include <iostream>
#include <mutex>    // 互斥量
#include <QByteArray>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>    // 用来读取config.ini配置

/***********************************************
    * @file     global.h
    * @brief    储存各种全局函数,包含项目中可能用到的各种头文件，在其他文件中仅包含global.h即可
    *
    * @author   wmlatiao
    * @date     2024/09/26
    * @history
***********************************************/

// extern关键字，说明refresh是在其他文件中定义的(并不是类中的成员函数，在头文件和源文件中的定义与实现有所区别)
extern std::function<void(QWidget*)> refresh;   // 用来刷新qss

extern std::function<QString(QString)> xorString;   // 对输入字符串进行异或操作加密密码

extern QString gate_url_prefix; // 设置一下gate url的前缀，在cpp文件中定义

enum ReqId{ // 记录几个http和tcp请求的id
    ID_GET_VARIFY_CODE = 1001,          //获取验证码
    ID_REG_USER = 1002,                 //注册用户
    ID_RESET_PWD = 1003,                //重置密码
    ID_LOGIN_USER = 1004,               //用户登录
    ID_CHAT_LOGIN = 1005,               //登陆聊天服务器
    ID_CHAT_LOGIN_RSP= 1006,            //登陆聊天服务器回包
    ID_SEARCH_USER_REQ = 1007,          //用户搜索请求
    ID_SEARCH_USER_RSP = 1008,          //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,           //添加好友申请
    ID_ADD_FRIEND_RSP = 1010,           //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,    //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,          //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,          //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015,   //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,       //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,       //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
};

enum Modules{   //记录不同的模块
    REGISTERMOD = 0,    //注册模块    
    RESETMOD = 1,       //重置密码模块
    LOGINMOD = 2,       //登录模块
};

enum ErrorCodes{    //记录错误信息，确定错误原因
    SUCCESS = 0,    //成功
    ERR_JSON = 1,   //json解析失败
    ERR_NETWORK = 2,    //网络错误

    // 下面是GateServer中设置的一些错误
    ERR_UserExist = 1005,   // 用户已存在
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,    // 确认密码的格式有误
    TIP_PWD_CONFIRM = 4,    // 确认密码与密码不匹配
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickLbState{  // 记录可点击标签的状态
    Normal = 0,     // 普通状态
    Selected = 1,   // 被选中状态
};

enum ChatUIMode{        // 定义聊天窗口的几种模式
    SearchMode,     // 搜索模式
    ChatMode,       // 聊天模式
    ContactMode,    // 联系人模式
};

enum ListItemType{          // 自定义ChatUserItem的几种类型
    CHAT_USER_ITEM,     // 聊天用户
    CONTACT_USER_ITEM,  // 联系人用户
    SEARCH_USER_ITEM,   // 搜索到的用户
    ADD_USER_TIP_ITEM,  // 提示添加用户
    INVALID_ITEM,       // 无效条目
    GROUP_TIP_ITEM,     // 分组提示条目
    LINE_ITEM,          // 分割线
    APPLY_FRIEND_ITEM,  // 好友申请
};

enum ChatRole{          // 当前对话widget所属角色
    Self,
    Other
};

struct ServerInfo{
    QString host;
    QString port;
    QString token;
    int uid;
};

struct MsgInfo{     // 要发送的消息的信息
    QString msgFlag;    // 表示消息类型，包括text、image、file等
    QString content;    // 表示文本信息、图像或文件的url等
    QPixmap pixmap;     // 图片或者文件的缩略图
};

// 申请好友标签输入最低长度
const int MIN_APPLY_LABEL_EDIT_LEN = 40;
const QString add_prefix = "添加标签：";
const int tip_offset = 5;

// 测试用数据，定义在chatdialog.cpp
extern std::vector<QString> heads;
extern std::vector<QString> strs;
extern std::vector<QString> names;


#endif // GLOBAL_H
