#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

#include <QFrame>
#include <QHBoxLayout>
#include "global.h"
#include <QPainter>
#include <QDebug>

/***********************************************
    * @file     bubbleframe.h
    * @brief    聊天气泡框框架，可通过继承实现不同种类消息，如文字、图片、音频等
    *
    * @author   wmlatiao
    * @date     2024/11/27
    * @history
***********************************************/

const int WIDTH_TRIANGLE = 8;    // 定义气泡框引出去的小三角宽度

class BubbleFrame : public QFrame
{
    Q_OBJECT
public:
    explicit BubbleFrame(ChatRole role, QWidget *parent = nullptr);
    void setMargin(const int& margin);
    void setWidget(QWidget* w); // 聊天内容是文本这里就是文本，内容为图片w就是图片

protected:
    void paintEvent(QPaintEvent* event) override;   // 给this绘制气泡

private:
    QHBoxLayout* _hLayout;   // 气泡框为水平布局
    ChatRole _role;
    int _margin;

signals:

public slots:
};

#endif // BUBBLEFRAME_H
