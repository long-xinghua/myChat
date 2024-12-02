#include "chatpage.h"
#include "ui_chatpage.h"
#include <QStyleOption>
#include <QPainter>
//#include "global.h"
#include "chatitembase.h"
#include "textbubble.h"
#include "picturebubble.h"

ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);

    // 给表情标签和文件标签初始化状态
    ui->emoLabel->setState("normal", "hover", "press", "normal", "hover", "press");
    ui->fileLabel->setState("normal", "hover", "press", "normal", "hover", "press");

    // 给接收和发送按钮初始化状态
    ui->recvButton->setState("normal", "hover", "press");
    ui->sendButton->setState("normal", "hover", "press");

}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;   // 定义样式选项,可以存储控件的状态、大小、启用/禁用状态等信息。
    opt.init(this);     // 用ChatPage的信息初始化opt
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);    // QStyle::PE_Widget 表示要绘制一个通用小部件背景
}

void ChatPage::on_sendButton_clicked()
{
    auto textEdit = ui->chatEdit;   // 获取到文字输入框
    // if(textEdit->toPlainText() == "") return;   // 内容为空就直接返回 在chatEdit里判断过就不再判断了

    ChatRole role = ChatRole::Self;
    QString userName = QStringLiteral("五毛辣条");
    QString userIcon = ":/res/head_1.jpg";

    const QVector<MsgInfo>& msgList = textEdit->getMsgList();
    for(int i=0; i<msgList.size(); ++i)
    {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;
        if(type == "text")
        {
            pBubble = new TextBubble(role, msgList[i].content);
        }
        else if(type == "image")
        {
            pBubble = new PictureBubble( role, QPixmap(msgList[i].content));
        }
        else if(type == "file")
        {

        }
        if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->msgWidget->appendChatItem(pChatItem);
        }
    }
}
