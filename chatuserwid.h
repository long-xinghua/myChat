#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"

/***********************************************
    * @file     chatuserwid.h
    * @brief    显示聊天用户的widget, 从继承自QWidget修改成继承自ListItemBase
    *
    * @author   wmlatiao
    * @date     2024/11/21
    * @history
***********************************************/

namespace Ui {
class ChatUserWid;
}


class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = 0);
    ~ChatUserWid();

    QSize sizeHint() const override;    // 返回自定义的尺寸
    void setInfo(QString name, QString head, QString msg);

private:
    Ui::ChatUserWid *ui;
    QString _name;          // 用户名
    QString _head;          // 头像路径
    QString _msg;           // 上次消息
};

#endif // CHATUSERWID_H
