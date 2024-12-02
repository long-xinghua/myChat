#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>

/***********************************************
    * @file     chatpage.h
    * @brief    界面右侧stackWidget中的聊天页面
    *
    * @author   wmlatiao
    * @date     2024/11/25
    * @history
***********************************************/

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = 0);
    ~ChatPage();

protected:
    void paintEvent(QPaintEvent* event) override;   // 重写paintEvent来设置背景颜色等，因为Qt没在QWidget里给接口来实现比较高级的喷绘功能

private slots:
    void on_sendButton_clicked();

private:
    Ui::ChatPage *ui;
};

#endif // CHATPAGE_H
