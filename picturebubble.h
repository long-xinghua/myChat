#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H

#include "bubbleframe.h"
#include <QLabel>
#include <QPixmap>

/***********************************************
    * @file     picturebubble.h
    * @brief    图片类型的聊天框
    *
    * @author   wmlatiao
    * @date     2024/11/28
    * @history
***********************************************/


class PictureBubble: public BubbleFrame
{
    Q_OBJECT
public:
    // 图片类型的消息实现构造就行了，不需要调整宽和高
    PictureBubble(ChatRole role, const QPixmap& picture,  QWidget *parent = nullptr);

private:
    QLabel* _picLabel;
};

#endif // PICTUREBUBBLE_H
