#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include "bubbleframe.h"

/***********************************************
    * @file     textbubble.h
    * @brief    文字类型的聊天框
    *
    * @author   wmlatiao
    * @date     2024/11/27
    * @history
***********************************************/

class TextBubble : public BubbleFrame   // 继承父类框架
{
    Q_OBJECT
public:
    explicit TextBubble(ChatRole role, const QString& text,  QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override; // 重写Paint事件，调用setTextHeight，调整气泡框的高度

private:
    void adjustTextHeight();                       // 调整气泡框的高度
    void setPlainText(const QString& text);     // 设置气泡框的最大宽度
    void initStyleSheet();

    QTextEdit* _textEdit;

signals:

public slots:
};

#endif // TEXTBUBBLE_H
