#ifndef AUTHFRIEND_H
#define AUTHFRIEND_H

#include <QDialog>
#include "clickedlabel.h"
#include "userdata.h"
#include "friendlabel.h"

/***********************************************
    * @file     authfriend.h
    * @brief    好友认证界面
    *
    * @author   wmlatiao
    * @date     2024/12/23
    * @history
***********************************************/

namespace Ui {
class AuthFriend;
}

class AuthFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AuthFriend(QWidget *parent = 0);
    ~AuthFriend();

    void initTipLbs();      // 初始化分组标签
    void addTipLbs(ClickedLabel*, QPoint cur_point, QPoint &next_point, int text_width, int text_height);   // 向标题展示列表中添加标签
    bool eventFilter(QObject *obj, QEvent *event) override;         // 捕获鼠标进入和离开事件，用于显示和隐藏滚动条
    void setSearchInfo(std::shared_ptr<SearchInfo> si);
    void setApplyInfo(std::shared_ptr<ApplyInfo> applyInfo);
private:
    Ui::AuthFriend *ui;
    //已经创建好的标签
    QMap<QString, ClickedLabel*> _add_labels;
    std::vector<QString> _add_label_keys;       // // 已经创建好了的标签控件名的容器，与_add_labels一致，可能会少于_tip_data
    std::vector<QString> _tip_data;             // 现有标签的容器，初始化时从服务器获取标签信息
    QPoint _tip_cur_point;                      // 计算展示框中label位置

    //用来在展示框显示给新好友添加的标签
    QMap<QString, friendLabel*> _friend_labels;
    std::vector<QString> _friend_label_keys;
     QPoint _label_point;                        // 计算给新好友添加新label的位置
    std::shared_ptr<SearchInfo> _si;
    std::shared_ptr<ApplyInfo> _applyInfo;

    void addLabel(QString name);                // 给新好友添加标签..........................这个函数有问题
    void resetLabels();  // 在删除新好友标签时重置标签



public slots:
    //显示更多label标签
    void slotShowMoreLabel();
    //输入label按下回车触发将标签加入展示栏
    void slotLabelEnter();
    //点击关闭，移除新好友便签
    void slotRemoveFriendLabel(QString);
    //通过点击tip实现增加和减少好友便签
    void slotChangeFriendLabelByTip(QString, ClickLbState);
    //输入框文本变化显示不同提示
    void slotLabelTextChange(const QString& text);
    //输入框输入完成
    void slotLabelEditFinished();
   //输入标签显示提示框，点击提示框内容后添加好友便签
    void slotAddFirendLabelByClickTip(QString text);
    //处理确认回调
    void slotApplySure();
    //处理取消回调
    void slotApplyCancel();
};

#endif // AUTHFRIEND_H
