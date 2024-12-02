#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H
#include <QWidget>
#include <QGridLayout>  // 网格布局
#include <QLabel>
#include "global.h"

/***********************************************
    * @file     chatiembase.h
    * @brief    聊天对话框基类，手写控件，不用.ui
    *
    * @author   wmlatiao
    * @date     2024/11/26
    * @history
***********************************************/


class BubbleFrame;  // 气泡框的基类

class ChatItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);
    void setUserName(const QString& name);
    void setUserIcon(const QPixmap& icon);
    void setWidget(QWidget* w);                 // 设置气泡框
private:
    ChatRole _role;         // 用户角色
    QLabel* _nameLabel;      // 昵称标签
    QLabel* _iconLabel;      // 头像标签
    QWidget* _bubble;        // 气泡对话框
    
signals:

public slots:
};

#endif // CHATIEMBASE_H
