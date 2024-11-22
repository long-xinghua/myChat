#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include <QLabel>
#include "global.h"


/***********************************************
    * @file     clickedlabel.h
    * @brief    能实现点击操作并得到相应反馈的按钮类
    *
    * @author   wmlatiao
    * @date     2024/10/31
    * @history
***********************************************/

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);
    ~ClickedLabel();

    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;      // 重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent* event) override;    // 重写鼠标释放事件

    void setState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");

    ClickLbState GetCurState();

private:
    // 未选中时的三个状态
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    // 选中时的三个状态
    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curState;

signals:
    void clicked();
};

#endif // CLICKEDLABEL_H
