#include "picturebubble.h"

// 设置图片的最大高和宽
#define PIC_MAX_WIDTH 160
#define PIC_MAX_HEIGHT 90

PictureBubble::PictureBubble(ChatRole role, const QPixmap &picture, QWidget *parent): BubbleFrame(role, parent)
{
    _picLabel = new QLabel();
    _picLabel->setScaledContents(true); // 设置自动缩放
    // _picLabel->adjustSize();    // 根据标签内容自动计算尺寸
    QPixmap pix = picture.scaled(QSize(PIC_MAX_WIDTH, PIC_MAX_HEIGHT), Qt::KeepAspectRatio);    // 固定标签的最大宽和高

    _picLabel->setPixmap(pix);
    this->setWidget(_picLabel);

    // 获取图片的实际大小
    QSize picSize = pix.size();

    // 固定气泡框的宽和高
    int left_margin = this->layout()->contentsMargins().left();
    int right_margin = this->layout()->contentsMargins().right();
    int v_margin = this->layout()->contentsMargins().bottom();

    // 之前用的_picLabel->height()和_picLabel->width(),它们没有及时更新导致获取到的尺寸与实际显示的内容不符。
    this->setFixedSize(picSize.width()+left_margin+right_margin, picSize.height()+v_margin*2);

}
