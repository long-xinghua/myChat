#include "clickedbtn.h"
#include "global.h"
#include <QVariant>

ClickedBtn::ClickedBtn(QWidget *parent):QPushButton(parent)
{
    setCursor(Qt::PointingHandCursor);      // 把光标设置成小手
    //this->setFocusPolicy(Qt::NoFocus);      // 把按钮设置为无焦点状态，就只能点击按钮触发，不能直接回车触发（控件不会接收键盘输入事件（例如 keyPressEvent 或 keyReleaseEvent），但它仍然可以接收鼠标事件）
    this->setFocusPolicy(Qt::ClickFocus);   // 只能通过鼠标点击获得焦点
}

ClickedBtn::~ClickedBtn()
{

}

void ClickedBtn::setState(QString normal, QString hover, QString press)
{
     _normal = normal;
     _hover = hover;
     _press = press;
     setProperty("state", _normal);  // 初始化成normal状态
     refresh(this);
     update();
}

void ClickedBtn::enterEvent(QEvent *event)
{
    setProperty("state", _hover);
    refresh(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
    setProperty("state", _normal);
    refresh(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent *event)
{
    setProperty("state", _press);
    refresh(this);
    update();
    QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *event)
{
    setProperty("state", _hover);
    refresh(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}
