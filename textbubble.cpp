#include "textbubble.h"
#include <QEvent>
#include <QTextBlock>

TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent): BubbleFrame(role, parent)
{
    _textEdit = new QTextEdit();
    _textEdit->setReadOnly(true);
    // 关闭滚动条，文字要完全呈现出来，这就要在后面设置一下this的高度（setTextHeight里），确保文字能全部显示
    _textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->installEventFilter(this);    // 给_textEdit安装事件过滤器，在this里拦截
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    _textEdit->setFont(font);
    setPlainText(text);         // 把文本内容添加到_textEdit
    setWidget(_textEdit);     // 父类BubbleFrame的方法，往水平布局里添加widget
    initStyleSheet();
}

bool TextBubble::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == _textEdit && event->type() == QEvent::Paint)  // 在改变界面大小时聊天框也会改变大小，同时会触发Paint事件，调整_textEdit中的内容
    {
        adjustTextHeight(); //PaintEvent中设置
    }
    return BubbleFrame::eventFilter(watched, event);
}

void TextBubble::adjustTextHeight()
{
    QTextDocument *doc = _textEdit->document();
    qreal doc_margin = doc->documentMargin();   //字体到边框的距离默认为4
    qreal text_height = 0;
    //把每一段的高度相加=文本高
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    // 文本是分行管理的，it是一行行的
    {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();                             //这段的rect
        text_height += text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();  // textEdit控件距离气泡框顶部的高度
    //设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
    this->setFixedHeight(text_height + doc_margin *2 + vMargin*2);
}

void TextBubble::setPlainText(const QString &text)
{
    _textEdit->setPlainText(text);
    //m_pTextEdit->setHtml(text);
    //找到段落中最大宽度
    qreal doc_margin = _textEdit->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    QFontMetricsF fm(_textEdit->font());
    QTextDocument *doc = _textEdit->document();
    int max_width = 0;
    //遍历每一段找到 最宽的那一段
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    //字体总长
    {
        int txtW = int(fm.width(it.text()));
        max_width = max_width < txtW ? txtW : max_width;                 //找到最长的那段
    }
    //设置这个气泡的最大宽度 只需要设置一次
    setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right));        //设置最大宽度
}

void TextBubble::initStyleSheet()
{
        _textEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}
