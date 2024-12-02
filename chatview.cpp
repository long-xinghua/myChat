#include "chatview.h"
#include <QScrollBar>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>

ChatView::ChatView(QWidget* parent): QWidget(parent), isAppended(false)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();   // 创建垂直布局
    this->setLayout(pMainLayout);                   // 将界面设置为垂直布局
    pMainLayout->setMargin(0);

    _pScrollArea = new QScrollArea();
    _pScrollArea->setObjectName("chatArea");    // 滚动区域名字叫chatArea，用这个名字去写qss
    pMainLayout->addWidget(_pScrollArea);       // 把滚动区域添加到垂直布局

    QWidget* w = new QWidget(this);             // 要放入滚动区域中的widget，在这个widget中添加内容
    w->setObjectName("chatBg");
    w->setAutoFillBackground(true);             // 自动填充背景

    QVBoxLayout* pVLayout_1 = new QVBoxLayout();// 在w中的垂直布局
    pVLayout_1->addWidget(new QWidget(), 10000);// 10000是缩放因子，这个加入的widget就固定占10000的大小，撑开滚动区域
    w->setLayout(pVLayout_1);
    _pScrollArea->setWidget(w);                 // 将w放入滚动区域中

    _pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 先关掉滚动区域里的滚动条
    QScrollBar* scrollBar = _pScrollArea->verticalScrollBar();          // 得到滚动区域里的竖直滚动条
    connect(scrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);  // 在滚动条范围变化（增加或删减条目）时进行处理

    // 创建水平布局
    QHBoxLayout* pHLayout = new QHBoxLayout();
    pHLayout->addWidget(scrollBar, 0, Qt::AlignRight);  // 把滚动条放在水平布局的右边
    pHLayout->setMargin(0);
    _pScrollArea->setLayout(pHLayout);           // 把滚动区域设成水平布局，左边的widget(w)中设为垂直布局，右边为滚动条
    scrollBar->setHidden(true);
    _pScrollArea->setWidgetResizable(true);
    _pScrollArea->installEventFilter(this);     // 给_pScrollArea安装事件过滤器，在this的eventFilter里捕获事件
    initStyleSheet();


}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout* vl = qobject_cast<QVBoxLayout*>(_pScrollArea->widget()->layout()); // 获取滚动区域中的widget(即w)的布局（垂直布局）
    vl->insertWidget(vl->count()-1, item);  // vl->count()指vl中已有的widget数量，因为在构造函数中插入过一个了就为1，1-1表示在第0个位置插入item
    // 插入新元素后会触发rangeChanged信号，调整滚动条位置
    isAppended = true;
}

void ChatView::prependChatItem(QWidget *item)
{
    QVBoxLayout* vl = qobject_cast<QVBoxLayout*>(_pScrollArea->widget()->layout()); // 获取滚动区域中的widget(即w)的布局（垂直布局）
    vl->insertWidget(0, item);
    // 插入新元素后会触发rangeChanged信号，调整滚动条位置
    isAppended = true;
}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

bool ChatView::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == _pScrollArea){
        if(event->type() == QEvent::Enter){
            // 鼠标进入，如果滚动条最大值为0就还是不显示，大于0则显示出来
            _pScrollArea->verticalScrollBar()->setHidden(_pScrollArea->verticalScrollBar()->maximum() == 0);
        }
        else if(event->type() == QEvent::Leave){
            _pScrollArea->verticalScrollBar()->setHidden(true);
        }
    }
    return QWidget::eventFilter(watched, event);

}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;   // 定义样式选项,可以存储控件的状态、大小、启用/禁用状态等信息。
    opt.init(this);     // 用ChatView的信息初始化opt
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);    // QStyle::PE_Widget 表示要绘制一个通用小部件背景
}

void ChatView::initStyleSheet()
{

}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended){ // 添加item可能调用多次
        QScrollBar* scrollBar = _pScrollArea->verticalScrollBar();
        scrollBar->setSliderPosition(scrollBar->maximum());
        // 500毫秒可能调用多次
        QTimer::singleShot(500, [this](){
            isAppended = false;
        });
    }
}
