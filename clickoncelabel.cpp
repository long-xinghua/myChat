#include "clickoncelabel.h"

ClickOnceLabel::ClickOnceLabel(QWidget *parent): QLabel(parent)
{
    this->setCursor(Qt::PointingHandCursor);
}

void ClickOnceLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        emit clicked(this->text());
        return;
    }
    QLabel::mouseReleaseEvent(ev);
}
