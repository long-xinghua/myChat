#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H

#include <QPushButton>

class ClickedBtn:public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget* parent = nullptr);
    ~ClickedBtn();
    void setState(QString normal, QString hover, QString press);    // 设置按钮的三种状态，正常状态、悬浮状态和按下状态
protected:
    // 重写父类中的虚函数
    virtual void enterEvent(QEvent* event) override;                // 重写鼠标进入
    virtual void leaveEvent(QEvent* event) override;                // 重写鼠标离开
    virtual void mousePressEvent(QMouseEvent* event) override;      // 重写鼠标按下
    virtual void mouseReleaseEvent(QMouseEvent* event) override;    // 重写鼠标释放
private:
    // 保存按钮的三个属性
    QString _normal;
    QString _hover;
    QString _press;
};

#endif // CLICKEDBTN_H
