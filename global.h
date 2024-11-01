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

enum ReqId{ // 记录几个http请求的功能
    ID_GET_VARIFY_CODE = 1001,  //获取验证码
    ID_REG_USER = 1002, //注册用户
};

enum Modules{   //记录不同的模块
    REGISTERMOD = 0,    //注册模块    
};

enum ErrorCodes{    //记录错误信息，确定错误原因
    SUCCESS = 0,    //成功
    ERR_JSON = 1,   //json解析失败
    ERR_NETWORK = 2,    //网络错误
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

extern QString gate_url_prefix; // 设置一下gate url的前缀，在cpp文件中定义


#endif // GLOBAL_H
