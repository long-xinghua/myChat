#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include "QStyle"

/***********************************************
    * @file     global.h
    * @brief    储存各种全局函数
    *
    * @author   wmlatiao
    * @date     2024/09/26
    * @history
***********************************************/

// extern关键字，说明refresh是在其他文件中定义的(并不是类中的成员函数，在头文件和源文件中的定义与实现有所区别)
extern std::function<void(QWidget*)> refresh;   // 用来刷新qss



#endif // GLOBAL_H
