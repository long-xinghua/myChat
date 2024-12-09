#ifndef FRIENDLABEL_H
#define FRIENDLABEL_H

#include <QFrame>

/***********************************************
    * @file     friendlabel.h
    * @brief    添加好友时给其指定的分类标签
    *
    * @author   wmlatiao
    * @date     2024/12/06
    * @history
***********************************************/

namespace Ui {
class friendLabel;
}

class friendLabel : public QFrame
{
    Q_OBJECT

public:
    explicit friendLabel(QWidget *parent = 0);
    ~friendLabel();

    void setText(const QString& text);
    int height();
    int width();
    QString text();

private:
    Ui::friendLabel *ui;
    QString _text;
    int _height;
    int _width;

public slots:
    void slot_close();
signals:
    void sig_close(QString);
};

#endif // FRIENDLABEL_H
