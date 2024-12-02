#ifndef CHATVIEW_H
#define CHATVIEW_H
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTimer>

/***********************************************
    * @file     chatview.h
    * @brief    ChatPage中中间显示聊天信息的widget,不用ui，纯手写界面
    *
    * @author   wmlatiao
    * @date     2024/11/23
    * @history
***********************************************/

class ChatView: public QWidget
{
    Q_OBJECT
public:
    ChatView(QWidget* parent = nullptr);
    void appendChatItem(QWidget* item);                     // 尾插元素
    void prependChatItem(QWidget* item);                    // 头插元素
    void insertChatItem(QWidget* before, QWidget* item);    // 中间位置插入
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;      // 事件过滤器，捕获鼠标进入、离开和滚轮事件
    void paintEvent(QPaintEvent* event) override;
private:
    void initStyleSheet();  // 初始化样式表

    QVBoxLayout* _pVl;          // 垂直布局，在垂直布局里添加item，用listWidget也能实现，两种方法都行(垂直布局就要自己添加滚动区域QScrollArea，QListWdiget默认就实现了滚动)
    QScrollArea* _pScrollArea;  // 滚动区域，内部包含了QScrollBar，且能通过滚动条的值自动调整内容的显示区域，通过滚动看到视窗之外的内容
    bool isAppended;            // 表示本次加载是否允许加载，因为加载完成之前不能因滚动过快加载更多次，防止出现卡顿或者数据错位
private slots:
    void onVScrollBarMoved(int min, int max);       // 在滚动条滚动时的一些响应
};

#endif // CHATVIEW_H
