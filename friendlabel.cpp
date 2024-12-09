#include "friendlabel.h"
#include "ui_friendlabel.h"

friendLabel::friendLabel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::friendLabel)
{
    ui->setupUi(this);
    ui->closeLabel->setState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");
    connect(ui->closeLabel, &ClickedLabel::clicked, this, &friendLabel::slot_close);
}

friendLabel::~friendLabel()
{
    delete ui;
}

void friendLabel::setText(const QString &text)
{
    _text = text;
    ui->tipLabel->setText(_text);
    ui->tipLabel->adjustSize();
    this->updateGeometry();

    // 得到tipLabel长度后根据这个来固定friendLabel的大小
    QFontMetrics fontMetrics(ui->tipLabel->font()); // 获取QLabel控件的字体信息
    auto textWidth = fontMetrics.width(ui->tipLabel->text()); // 获取文本的宽度
    auto textHeight = fontMetrics.height(); // 获取文本的高度，没有用到

    qDebug()<< " ui->tipLabel.width() is " << ui->tipLabel->width();
    qDebug()<< " ui->tipLabel.height() is " << ui->tipLabel->height();
    qDebug()<< " textWidth is " << textWidth;
    qDebug()<< " textHeight is " << textHeight;
    this->setFixedWidth(ui->tipLabel->width()+ui->closeWidget->width()+5);   // 设置firendLabel宽度
    this->setFixedHeight(textHeight+2);                                     // 设置friendLabel高度

    this->updateGeometry();
    this->repaint();

    // 为什么得到的大小是完全错的？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
    qDebug()<<  "  this->setFixedWidth " << this->width();
     qDebug()<< "  this->setFixedHeight " << this->height();
    _width = this->width();
    _height = this->height();
}

int friendLabel::height()
{
    return _height;
}

int friendLabel::width()
{
    return _width;
}

QString friendLabel::text()
{
    return _text;
}

void friendLabel::slot_close()
{
    emit sig_close(_text);  // 相当于点击关闭标签时又触发sig_close信号
}
