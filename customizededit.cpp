#include "customizededit.h"

CustomizedEdit::CustomizedEdit(QWidget *parent):QLineEdit(parent), _maxLen(0)
{
    // 每当输入框中内容改变的时候就调用限制文本长度的函数
    connect(this, &QLineEdit::textChanged, this, &CustomizedEdit::limitTextLength);
}

void CustomizedEdit::setMaxLength(int maxLen)
{
    _maxLen = maxLen;
}

void CustomizedEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);    // 确保基类中的行为正常执行
    emit sig_focus_out();
}

void CustomizedEdit::limitTextLength(QString text)
{
    if(_maxLen <= 0) return;
    QByteArray byteArray = text.toUtf8();               // 转换成字节流方便统计字节量
    if(byteArray.size() > _maxLen){
        byteArray = byteArray.left(_maxLen);            // 截取到最大长度，这里会有bug，如果一次性输入超过限制的中文会乱码且无法删除
        this->setText(QString::fromUtf8(byteArray));
    }
}
