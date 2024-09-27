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
};


#endif // GLOBAL_H
