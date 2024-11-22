#ifndef CUSTOMIZEDEDIT_H
#define CUSTOMIZEDEDIT_H
#include <QLineEdit>
#include <QDebug>

/***********************************************
    * @file     customizededit.h
    * @brief    自定义搜索框类
    *
    * @author   wmlatiao
    * @date     2024/11/21
    * @history
***********************************************/

class CustomizedEdit: public QLineEdit
{
    Q_OBJECT
public:
    CustomizedEdit(QWidget* parent=nullptr);
    void setMaxLength(int maxLen);
protected:
    void focusOutEvent(QFocusEvent* event) override;    // 失去焦点时发送一个信号
private:
    void limitTextLength(QString text);

    int _maxLen;
signals:
    void sig_focus_out();
};

#endif // CUSTOMIZEDEDIT_H
