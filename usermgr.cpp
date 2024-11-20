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


