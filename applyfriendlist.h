#ifndef APPLYFRIENDLIST_H
#define APPLYFRIENDLIST_H

#include <QListWidget>
#include <QEvent>

/***********************************************
    * @file     applyfriendlist.h
    * @brief    好友申请列表的类
    *
    * @author   wmlatiao
    * @date     2024/12/11
    * @history
***********************************************/

class ApplyFriendList: public QListWidget
{
    Q_OBJECT
public:
    ApplyFriendList(QWidget* parent = nullptr);
protected:
    bool eventFilter(QObject *, QEvent *) override;
public slots:

signals:
    void sig_show_search(bool s);
};

#endif // APPLYFRIENDLIST_H
