#include "bubbleframe.h"

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent) : QFrame(parent), _role(role), _margin(3)
{
    _hLayout = new QHBoxLayout();
    if(_role == ChatRole::Self){
        // 角色是自己，气泡右边留出给气泡小三角的空白
        _hLayout->setContentsMargins(_margin, _margin, WIDTH_TRIANGLE+_margin, _margin);
    }
    else{   // 气泡左边留出空白
        _hLayout->setContentsMargins(WIDTH_TRIANGLE+_margin, _margin, _margin, _margin);
    }

    this->setLayout(_hLayout);
}

void BubbleFrame::setMargin(const int& margin)
{
    Q_UNUSED(margin);
}

void BubbleFrame::setWidget(QWidget *w)
{
    if(_hLayout->count() > 0) return;
    else{
        _hLayout->addWidget(w);
    }
}

void BubbleFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);      // 不用画笔，画出来的东西就没有轨迹（边框）

    if(_role == ChatRole::Other)
    {
        //画气泡矩形
        QColor bk_color(Qt::white); // 别人发的消息为白色
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_TRIANGLE, 0, this->width()-WIDTH_TRIANGLE, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        //画小三角
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10+WIDTH_TRIANGLE +2),
            QPointF(bk_rect.x()-WIDTH_TRIANGLE, 10+WIDTH_TRIANGLE-WIDTH_TRIANGLE/2),
        };
        painter.drawPolygon(points, 3);
    }
    else
    {
        QColor bk_color(158,234,106);   // 自己发的消息为绿色
        painter.setBrush(QBrush(bk_color));
        //画气泡矩形
        QRect bk_rect = QRect(0, 0, this->width()-WIDTH_TRIANGLE, this->height());  // 右边留出给小三角的宽度
        painter.drawRoundedRect(bk_rect,5,5);   // 画上圆角矩形
        //画三角
        QPointF points[3] = {
            QPointF(bk_rect.x()+bk_rect.width(), 12),
            QPointF(bk_rect.x()+bk_rect.width(), 12+WIDTH_TRIANGLE +2),
            QPointF(bk_rect.x()+bk_rect.width()+WIDTH_TRIANGLE, 10+WIDTH_TRIANGLE-WIDTH_TRIANGLE/2),
        };
        painter.drawPolygon(points, 3);

    }

    return QFrame::paintEvent(event);
}
