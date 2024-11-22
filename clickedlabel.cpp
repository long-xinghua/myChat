#include "clickedlabel.h"
#include <QMouseEvent>


ClickedLabel::ClickedLabel(QWidget *parent): QLabel(parent), _curState(ClickLbState::Normal)    // 初始状态为未选中的Normal状态
{
    // 这样当鼠标进入clickedLabel对象时光标就会自动变成小手的样子
    this->setCursor(Qt::PointingHandCursor);
}

ClickedLabel::~ClickedLabel()
{

}

void ClickedLabel::enterEvent(QEvent *event)
{
    if(_curState == ClickLbState::Normal){
        qDebug()<<"进入标签，切换至未选中悬浮状态: "<<_normal_hover;
        setProperty("state", _normal_hover);
        refresh(this);
        update();
    }else{
        qDebug()<<"进入标签，切换至选中悬浮状态: "<<_selected_hover;
        setProperty("state", _selected_hover);
        refresh(this);
        update();
    }
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    if(_curState == ClickLbState::Normal){
        qDebug()<<"离开标签，切换至未选中状态: "<<_normal;
        setProperty("state", _normal);
        refresh(this);
        update();
    }else{
        qDebug()<<"进入标签，切换至选中状态: "<<_selected;
        setProperty("state", _selected);
        refresh(this);
        update();
    }
}

void ClickedLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(_curState == ClickLbState::Normal){
            qDebug()<<"鼠标按下，标签切换至选中按下状态: "<<_selected_press;
            _curState = ClickLbState::Selected;
            setProperty("state", _selected_press);  // 这时鼠标刚按下还未释放，因此设置成按下状态
            refresh(this);
            update();
        }else{
            qDebug()<<"鼠标按下，标签切换至未选中按下状态: "<<_selected_press;
            _curState = ClickLbState::Normal;
            setProperty("state", _normal_press);
            refresh(this);
            update();
        }
    }

    // 特殊处理做完后还要调用一下基类的函数保证其他效果能正确执行
    QLabel::mousePressEvent(event);
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(_curState == ClickLbState::Normal){
            qDebug()<<"鼠标释放，标签切换至选中悬浮状态: "<<_selected_hover;
            _curState = ClickLbState::Selected;
            setProperty("state", _selected_hover);  // 这时鼠标刚按下还未离开，因此设置成按下且悬浮的状态
            refresh(this);
            update();
        }else{
            qDebug()<<"鼠标释放，标签切换至未选中悬浮状态: "<<_selected_hover;
            _curState = ClickLbState::Normal;
            setProperty("state", _normal_hover);
            refresh(this);
            update();
        }
        emit clicked(); // 触发点击信号，槽函数来切换一下密码的可见性
    }

    // 特殊处理做完后还要调用一下基类的函数保证其他效果能正确执行
    QLabel::mouseReleaseEvent(event);
}

void ClickedLabel::setState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;
    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state", normal);
    refresh(this);
}

ClickLbState ClickedLabel::GetCurState()
{
    return _curState;
}
