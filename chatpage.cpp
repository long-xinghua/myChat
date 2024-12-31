#include "chatpage.h"
#include "ui_chatpage.h"
#include <QStyleOption>
#include <QPainter>
//#include "global.h"
#include "chatitembase.h"
#include "textbubble.h"
#include "picturebubble.h"
#include "tcpmgr.h"
#include <QUuid>
#include <QJsonDocument>


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

void ChatPage::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    if(_userInfo == userInfo) return;
    _userInfo = userInfo;
    // 设置ui界面 
    ui->titleLabel->setText(_userInfo->_name);
    ui->msgWidget->removeAllItem();
    for(auto& msg:userInfo->_chat_msgs){
        appendChatMsg(msg);
    }
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;   // 定义样式选项,可以存储控件的状态、大小、启用/禁用状态等信息。
    opt.init(this);     // 用ChatPage的信息初始化opt
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);    // QStyle::PE_Widget 表示要绘制一个通用小部件背景
}

void ChatPage::appendChatMsg(std::shared_ptr<TextChatData> msg)
{
    auto self_info = UserMgr::GetInstance()->getUserInfo();
    ChatRole role;
    //todo... 添加聊天显示
    if (msg->_from_uid == self_info->_uid) {
        role = ChatRole::Self;
        // 设置要插入的widget
        ChatItemBase* pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(self_info->_name);
        pChatItem->setUserIcon(QPixmap(self_info->_icon));
        // 设置气泡框
        QWidget* pBubble = nullptr;
        pBubble = new TextBubble(role, msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->msgWidget->appendChatItem(pChatItem);
    }
    else {
        role = ChatRole::Other;
        // 设置要插入的widget
        ChatItemBase* pChatItem = new ChatItemBase(role);
        auto friend_info = UserMgr::GetInstance()->getFriendByUid(msg->_from_uid);
        if (friend_info == nullptr) {
            return;
        }
        pChatItem->setUserName(friend_info->_name);
        pChatItem->setUserIcon(QPixmap(friend_info->_icon));
        // 设置气泡框
        QWidget* pBubble = nullptr;
        pBubble = new TextBubble(role, msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->msgWidget->appendChatItem(pChatItem);
    }
}

void ChatPage::on_sendButton_clicked()
{
    if(_userInfo == nullptr){
        return;
    }
    auto textEdit = ui->chatEdit;   // 获取到文字输入框
    // if(textEdit->toPlainText() == "") return;   // 内容为空就直接返回 在chatEdit里判断过就不再判断了

    ChatRole role = ChatRole::Self;
    auto selfInfo = UserMgr::GetInstance()->getUserInfo();
    QString userName = selfInfo->_name;
    QString userIcon = selfInfo->_icon;

    const QVector<MsgInfo>& msgList = textEdit->getMsgList();
    // 做好准备将json数据发给服务器
    QJsonObject textObj;
    QJsonArray textArray;
    int text_size=0;


    // 发的消息中如果又有文字又有图片之类的msgList就会有多个元素，但如果只有文字其实msgList就一个元素
    for(int i=0; i<msgList.size(); ++i)
    {
        //消息内容长度不合规就跳过
        if(msgList[i].content.length() > 1024){
            qDebug()<<"内容长度过长";
            continue;
        }
        // 获取消息类型
        QString type = msgList[i].msgFlag;
        // 在chatView上添加聊天框
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;

        if(type == "text")
        {
            //生成唯一id
            QUuid uuid = QUuid::createUuid();
            //转为字符串
            QString uuidString = uuid.toString();

            pBubble = new TextBubble(role, msgList[i].content);
            // 如果加上这条消息长度超过1024了就先把之前的消息发给服务器，清空textArray后再放入该消息
            if(text_size + msgList[i].content.length()> 1024){
                textObj["from_uid"] = selfInfo->_uid;
                textObj["to_uid"] = _userInfo->_uid;
                textObj["text_array"] = textArray;
                QJsonDocument doc(textObj);
                QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
                //发送并清空之前累计的文本列表
                text_size = 0;
                textArray = QJsonArray();
                textObj = QJsonObject();
                //发送tcp请求给chat server
                emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
            }

            //将bubble和uid绑定，以后可以等网络返回消息后设置是否送达
            //_bubble_map[uuidString] = pBubble;
            text_size += msgList[i].content.length();
            QJsonObject obj;
            QByteArray utf8Message = msgList[i].content.toUtf8();
            obj["content"] = QString::fromUtf8(utf8Message);
            obj["msgid"] = uuidString;
            textArray.append(obj);
            auto text_msg = std::make_shared<TextChatData>(uuidString, obj["content"].toString(),
                selfInfo->_uid, _userInfo->_uid);
            emit sig_append_send_chat_msg(text_msg);
        }
        else if(type == "image")
        {
             pBubble = new PictureBubble(role, QPixmap(msgList[i].content));
        }
        else if(type == "file")
        {

        }
        //发送消息
        if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->msgWidget->appendChatItem(pChatItem);
        }

    }

    qDebug() << "textArray is " << textArray ;
    //发送给服务器
    textObj["text_array"] = textArray;
    textObj["from_uid"] = selfInfo->_uid;
    textObj["to_uid"] = _userInfo->_uid;
    QJsonDocument doc(textObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    //发送并清空之前累计的文本列表
    text_size = 0;
    textArray = QJsonArray();
    textObj = QJsonObject();
    //发送tcp请求给chat server
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
}
