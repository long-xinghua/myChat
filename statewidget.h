#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>
#include "global.h"
#include <QLabel>

/***********************************************
    * @file     statewidget.h
    * @brief    聊天界面左边侧边栏中小控件的类，有多种状态，有新消息时能显示红点
    *
    * @author   wmlatiao
    * @date     2024/12/02
    * @history
***********************************************/


class StateWidget: public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);

    void setState(QString normal="", QString hover="", QString press="",                    // 设置状态名
                  QString select="", QString select_hover="", QString select_press="");

    ClickLbState getCurState();
    void clearState();                  // 重置状态到normal

    void setSelected(bool bselected);   // 在选中和未选中之间切换
    void addRedPoint();                 // 加上红点
    void showRedPoint(bool show=true);  // 展示红点

protected:
    void paintEvent(QPaintEvent* event);

    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;


private:

    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curstate;
    QLabel * _red_point;

signals:
    void clicked(StateWidget* w);

signals:

public slots:
};

#endif // STATEWIDGET_H
