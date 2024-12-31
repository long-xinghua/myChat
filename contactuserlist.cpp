#include "contactuserlist.h"
#include "global.h"
#include "listitembase.h"
#include "grouptipitem.h"
#include "contactuseritem.h"
#include <random>
#include "tcpmgr.h"
#include "usermgr.h"
#include "userdata.h"

ContactUserList::ContactUserList(QWidget *parent): QListWidget(parent), _add_friend_item(nullptr), _groupItem(nullptr)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);

    // 模拟从后端传过来的数据进行列表加载
    addContactUserList();
    // 连接点击物品的信号和槽
    connect(this, &ContactUserList::itemClicked, this, &ContactUserList::slot_item_clicked);
    // 连接对端同意认证后通知的信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend, this, &ContactUserList::slot_add_auth_friend);
    // 连接自己同意认证并拿到服务器回包后的处理（界面刷新）
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ContactUserList::slot_auth_rsp);

}

ContactUserList::~ContactUserList()
{
    qDebug()<<"ContactUserList 析构";
}

void ContactUserList::showRedPoint(bool show)
{
    _add_friend_item->showRedPoint(show);
}

bool ContactUserList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量

        if (maxScrollValue - currentValue <= 0) {
            // 滚动到底部，加载新的联系人
            qDebug()<<"load more contact user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_contact_user();
         }

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}



void ContactUserList::addContactUserList()
{
    // 新建一个分组item
    auto * groupTip = new GroupTipItem();
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(groupTip->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, groupTip);
    // 加上~，代表这是不可选中的item
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    // 添加一个用于添加好友的item
    _add_friend_item = new ContactUserItem();
    _add_friend_item->setObjectName("new_friend_item");
    _add_friend_item->setInfo(0, tr("新的朋友"),":/res/add_friend.png");
    _add_friend_item->setItemType(ListItemType::APPLY_FRIEND_ITEM);         // 用的是联系人项的类，默认type为联系人的type，改一下
    QListWidgetItem *add_item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    add_item->setSizeHint(_add_friend_item->sizeHint());
    this->addItem(add_item);
    this->setItemWidget(add_item, _add_friend_item);
    //默认设置新的朋友申请条目被选中
    this->setCurrentItem(add_item);

    // 添加新分组
    auto * groupCon = new GroupTipItem();
    groupCon->setGroupTip(tr("联系人"));
    QListWidgetItem *groupitem = new QListWidgetItem;
    groupitem->setSizeHint(groupCon->sizeHint());
    this->addItem(groupitem);
    this->setItemWidget(groupitem, groupCon);
    groupitem->setFlags(groupitem->flags() & ~Qt::ItemIsSelectable);
    //..................................................................................................
    _groupItem = groupitem;


    // 模拟数据
//    for(int i = 0; i < 13; i++){
//        //int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
//        int randomValue = std::rand() % 100 + 1;
//        int str_i = randomValue%strs.size();
//        int head_i = randomValue%heads.size();
//        int name_i = randomValue%names.size();

//        auto *con_user_wid = new ContactUserItem();
//        con_user_wid->setInfo(0, names[name_i], heads[head_i]);
//        QListWidgetItem *item = new QListWidgetItem;
//        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
//        item->setSizeHint(con_user_wid->sizeHint());
//        this->addItem(item);
//        this->setItemWidget(item, con_user_wid);
//    }
}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); // 获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    // 转换失败会返回一个空指针
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->getItemType();
    // 获取item类型
    if(itemType == ListItemType::INVALID_ITEM
            || itemType == ListItemType::GROUP_TIP_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

   if(itemType == ListItemType::APPLY_FRIEND_ITEM){

       qDebug()<< "apply friend item clicked ";
       //跳转到好友申请界面
       emit sig_switch_apply_friend_page();
       return;
   }

   if(itemType == ListItemType::CONTACT_USER_ITEM){
       qDebug()<<"contact user item clicked ";
       ContactUserItem* contactItem = qobject_cast<ContactUserItem*>(customItem);
       // 跳转到联系人详情页面
       emit sig_switch_friend_info_page(contactItem->getInfo());
       return;
   }
}

void ContactUserList::slot_add_auth_friend(std::shared_ptr<AuthInfo> authInfo)
{
    qDebug()<<"slot_add_auth_friend called";
    // 判断对方是否已经在好友列表中了
    bool isFriend = UserMgr::GetInstance()->checkFriendByUid(authInfo->_uid);
    if(isFriend){
        return;
    }

    auto* con_user_wid = new ContactUserItem();
    con_user_wid->setInfo(authInfo);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(con_user_wid->sizeHint());

    // 获取groupItem的索引
    int index = this->row(_groupItem);
    // 在groupItem后面插入新联系人
    this->insertItem(index+1, item);
    this->setItemWidget(item, con_user_wid);

}

void ContactUserList::slot_auth_rsp(std::shared_ptr<AuthRsp> authRsp)
{
    qDebug()<<"slot_auth_rsp called";
    // 判断对方是否已经在好友列表中了
    bool isFriend = UserMgr::GetInstance()->checkFriendByUid(authRsp->_uid);
    if(isFriend){
        return;
    }

    auto* con_user_wid = new ContactUserItem();
    con_user_wid->setInfo(authRsp);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(con_user_wid->sizeHint());

    // 获取groupItem的索引
    int index = this->row(_groupItem);
    // 在groupItem后面插入新联系人
    this->insertItem(index+1, item);
    this->setItemWidget(item, con_user_wid);
}
