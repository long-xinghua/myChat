#include "searchlist.h"
#include<QScrollBar>
#include "adduseritem.h"
//#include "invaliditem.h"
#include "searchresultdialog.h"
#include "tcpmgr.h"
#include "customizededit.h"
#include "searchfaileddialog.h"
#include <QJsonDocument>
#include "usermgr.h"


SearchList::SearchList(QWidget *parent):QListWidget(parent),_searchResultDlg(nullptr), _search_edit(nullptr), _send_pending(false)
{
    //Q_UNUSED(parent);
     this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);
    //连接点击物品的信号和槽
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_item_clicked);
    //添加条目
    addTipItem();
    //连接搜索条目
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::closeFindDlg()
{
    if(_searchResultDlg){
        _searchResultDlg->hide();
        _searchResultDlg = nullptr;
    }
}

void SearchList::setSearchEdit(QWidget *edit)
{
    _search_edit = edit;
}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
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

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}

void SearchList::waitPending(bool pending)
{
    if(pending){    // 说明现在处于发送请求中的状态
        _loadingDialog = new LoadingDialog(this);
        _loadingDialog->setModal(true);
        //qDebug()<<"展示加载对话框";
        _loadingDialog->show();
        //qDebug()<<"_loadingDialog->show()";
        _send_pending = pending;
    }
    else{           // 未处于请求状态
        _loadingDialog->hide();
        _loadingDialog->deleteLater();
        _send_pending = pending;
    }
}

void SearchList::addTipItem()
{
    // 先添加一个空控件到list中
    auto *invalid_item = new QWidget();
    QListWidgetItem *item_tmp = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    this->setItemWidget(item_tmp, invalid_item);
    item_tmp->setFlags(item_tmp->flags() & ~Qt::ItemIsSelectable);  // 将item设为可选中的

    // 添加一个搜索条目用于测试，点击该条目再弹出具体搜索结果
    auto *add_user_item = new AddUserItem();
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    // 把item转为QWidget
    QWidget* w = this->itemWidget(item);
    // 将w转成ListItemBase类型
    ListItemBase* myItem = qobject_cast<ListItemBase*>(w);
    // 转换失败会返回空指针
    if(!myItem){
        qDebug()<<"点击了空部件";
        return;
    }
    // 根据ListItemBase中的getItemType获取物体类型，执行对应操作
    ListItemType itemType = myItem->getItemType();
    if(itemType == ListItemType::INVALID_ITEM){               // 无效条目
        qDebug()<<"点击到无效条目";
        return;
    }
    else if(itemType == ListItemType::ADD_USER_TIP_ITEM){     // 添加/查找好友条目
        if(_send_pending) { // 如果正在查找中就直接返回，防止重复点击
            return;
        }
        waitPending(true);
        // 使用_search_edit前需初始化，否则会崩
        if(_search_edit == nullptr){
            qDebug()<<"_search_edit为空！";
            return;
        }
        auto search_edit = dynamic_cast<CustomizedEdit*>(_search_edit);
        auto uid_str = search_edit->text();     // 实际上搜索的可能是uid也可能是昵称
        //此处发送请求给server
        QJsonObject jsonObj;
        jsonObj["uid"] = uid_str;               // 将uid(或昵称)加入到发送信息里

        QJsonDocument doc(jsonObj);
        QByteArray jsonString = doc.toJson(QJsonDocument::Compact);    // 发送压缩的json，减少空格，降低数据量

        //发送tcp请求给chat server
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonString);

//        // 还没连接服务器，简单弹出对话框做个测试
//        _searchResultDlg = std::make_shared<SearchResultDialog>(this);
//        std::shared_ptr<SearchInfo> si = std::make_shared<SearchInfo>(0, "张三", "歪比巴卜", "hello", 1);
//        std::dynamic_pointer_cast<SearchResultDialog>(_searchResultDlg)->setSearchInfo(si);
//        _searchResultDlg->show();
        return;

    }


    closeFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);     // 搜索状态结束
    if(si == nullptr){      // 说明搜索失败
        _searchResultDlg = std::make_shared<SearchFailedDialog>(this);
    }
    else{                   // 搜索成功
        // 判断搜索的用户是否是自己
        if(si->_uid == UserMgr::GetInstance()->getUid()){
            auto searchFailedDlg = new SearchFailedDialog(this);
            searchFailedDlg->setTipLabel("查找用户为本用户");
            searchFailedDlg->setTipLabel_2("无法添加自身为好友");
            searchFailedDlg->show();
            return;
        }
        // 判断是否已经添加了好友
        bool isFriend = UserMgr::GetInstance()->checkFriendByUid(si->_uid);
        // 是好友则直接跳转到与其的聊天界面
        if(isFriend){
            emit sig_jump_chat_item(si);
            return;
        }
        _searchResultDlg = std::make_shared<SearchResultDialog>(this);
        std::dynamic_pointer_cast<SearchResultDialog>(_searchResultDlg)->setSearchInfo(si);
    }
    _searchResultDlg->show();
}
