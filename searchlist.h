#ifndef SEARCHLIST_H
#define SEARCHLIST_H
#include <QListWidget>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>
#include <memory>
#include "loadingdialog.h"
#include "userdata.h"

/***********************************************
    * @file     searchlist.h
    * @brief    chatDialog中的搜索列表
    *
    * @author   wmlatiao
    * @date     2024/12/03
    * @history
***********************************************/


class SearchList: public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget *parent = nullptr);
    void closeFindDlg();                // 关闭查找对话框
    void setSearchEdit(QWidget* edit);  // 传入ChatDialog中的searchEdit，获取输入框中的内容
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    bool _send_pending;                 // 为true时界面为阻塞状态，收到服务器回包后置为false
    std::shared_ptr<QDialog> _searchResultDlg; // 查找的对话框
    QWidget* _search_edit;
    LoadingDialog * _loadingDialog;             // 显示转圈加载的界面

    void waitPending(bool pending = true);      // 根据pending情况显示模态加载对话框和更新_send_pending状态
    void addTipItem();                 // 模拟从服务器获取到条目的情况，测试用
signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
private slots:
    void slot_item_clicked(QListWidgetItem *item);          // 点击搜索列表中的物品槽函数
    void slot_user_search(std::shared_ptr<SearchInfo> si);  // 搜索用户回包的回调函数
signals:

};

#endif // SEARCHLIST_H
