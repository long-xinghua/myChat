#ifndef USERDATA_H
#define USERDATA_H
#include <QString>

/***********************************************
    * @file     userdata.h
    * @brief    管理用户信息的类，用于搜索
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

#endif // USERDATA_H
