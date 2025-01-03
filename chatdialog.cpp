#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
// #include <QRandomGenerator> Qt版本较低，没有这个库，用标准库里的随机数生成器代替
#include <random>
#include <ctime>
#include "chatuserwid.h"
#include <vector>
#include "loadingdialog.h"
#include "tcpmgr.h"
#include "usermgr.h"
#include "contactuseritem.h"


ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent), _mode(ChatUIMode::ChatMode), _isSearching(false), _b_loading(false), _cur_chat_uid(0),_lastWidget(nullptr),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    ui->addButton->setState("normal", "hover", "press");    // 初始化addButton的状态标签
    ui->searchEdit->setMaxLength(20);                       // 设置searchEdit中的最大文本长度

    // 创建一个搜索action，给搜索框加上搜索图标
    QAction* searchAction = new QAction(ui->searchEdit);                    // searchAction生命周期由searchEdit管理
    searchAction->setIcon(QIcon(":res/search.png"));                        // 给searchAction设置图标
    ui->searchEdit->addAction(searchAction, QLineEdit::LeadingPosition);    // 将 searchAction添加到QLineEdit的前置位置
    ui->searchEdit->setPlaceholderText(QStringLiteral("搜索"));              // 设置searchEdit的占位文本

    // 创建一个清除action，给搜索框加上清除图标，并在按下时清除搜索框内容
    QAction* clearAction = new QAction(ui->searchEdit);
    clearAction->setIcon(QIcon(":res/close_transparent.png"));                     // 先设置一个透明图标，在有文本内容时才改成不透明图标
    ui->searchEdit->addAction(clearAction, QLineEdit::TrailingPosition);    // 将clearAction放到searchEdit的末尾位置
    // 当文字有变化时将透明图标改成不透明的
    connect(ui->searchEdit, &CustomizedEdit::textChanged, [clearAction](const QString& text){
            if(!text.isEmpty()){    // 搜索框内容不为空
                clearAction->setIcon(QIcon(":/res/close_search.png"));
            }else{
                clearAction->setIcon(QIcon(":res/close_transparent.png"));
            }
    });

    connect(clearAction, &QAction::triggered, [this, clearAction](){    // 为什么lambda表达式不需要有bool类型的参数？triggered的参数类型是bool的
        ui->searchEdit->clear();    // 清除搜索框内容
        ui->searchEdit->clearFocus();   // 让搜索框失去焦点,即不处于待输入状态了
        showList(_mode);            // 隐藏搜索列表
    });


    // 连接加载聊天记录和联系人的信号
    connect(ui->chatUserList, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    connect(ui->contactList, &ContactUserList::sig_loading_contact_user, this, &ChatDialog::slot_loading_contact_user);

    // 设置侧边栏中的头像
    QPixmap head(UserMgr::GetInstance()->getIcon());
    head = head.scaled(ui->sideHeadLabel->size(), Qt::KeepAspectRatio);
    ui->sideHeadLabel->setPixmap(head);
    //ui->sideHeadLabel->setScaledContents(true);

    // 设置侧边栏中小控件的状态
    ui->sideChatLabel->setState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");
    ui->sideContactLabel->setState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");
    ui->sideChatLabel->setSelected(true);   // 默认为聊天界面
    // 将控件加到组里
    addLabelGroup(ui->sideChatLabel);
    addLabelGroup(ui->sideContactLabel);

    connect(ui->sideChatLabel, &StateWidget::clicked, this, &ChatDialog::slot_sideWid_clicked);
    connect(ui->sideContactLabel, &StateWidget::clicked, this, &ChatDialog::slot_sideWid_clicked);

    // 搜索框内容变化时的响应
    connect(ui->searchEdit, &CustomizedEdit::textChanged, this, &ChatDialog::slot_text_changed);


    // 安装事件过滤器，手动捕获一些事件,如检测鼠标点击位置判断是否清除搜索框
    this->installEventFilter(this);

    //showList(_mode);    // 默认情况下为聊天模式，隐藏搜索列表
    emit ui->sideChatLabel->clicked(ui->sideChatLabel); // 默认开始时为聊天界面，相当于先点击了一次聊天标签

    // 设置当前选中的项和聊天界面
    this->setSelectChatItem();
    this->setSelectChatPage();

    // 给searchList传入searchEdit
    ui->searchList->setSearchEdit(ui->searchEdit);

    // 连接新好友申请的信号和槽
    connect(TcpMgr::GetInstance().get(), TcpMgr::sig_firend_apply, this, ChatDialog::slot_apply_friend);

    // 连接对方认证好友通过信号
    connect(TcpMgr::GetInstance().get(), TcpMgr::sig_add_auth_friend, this, ChatDialog::slot_add_auth_firend);
    // 连接认证通过对方好友申请信号
    connect(TcpMgr::GetInstance().get(), TcpMgr::sig_auth_rsp, this, ChatDialog::slot_auth_rsp);
    //  连接跳转好友聊天信号
    connect(ui->searchList, &SearchList::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item_from_search);
    connect(ui->friendInfoPage, &FriendInfoPage::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item_from_infoPage);
    // 往chatUserList添加真实数据
    addUserList();
    // 往contactList添加真实数据
    addContactList();
    // 在联系人列表点击新好友和联系人的跳转
    connect(ui->contactList, &ContactUserList::sig_switch_friend_info_page, this, &ChatDialog::slot_switch_friend_info_page);
    connect(ui->contactList, &ContactUserList::sig_switch_apply_friend_page, this, &ChatDialog::slot_switch_apply_friend_page);
    
    // 在聊天列表点击聊天项跳转到对应的聊天页面
    connect(ui->chatUserList, &ChatUserList::itemClicked, this, &ChatDialog::slot_chatList_item_clicked);

    connect(ui->chatPage, &ChatPage::sig_append_send_chat_msg, this, &ChatDialog::slot_append_send_chat_msg);
    
    connect(TcpMgr::GetInstance().get(), TcpMgr::sig_text_chat_msg, this, &ChatDialog::slot_text_chat_msg);

    // 下面这里测试用,往chatUserList添加模拟数据
    //addUserListTest();
}

ChatDialog::~ChatDialog()
{
    delete ui;
}


// 用于测试的数据
std::vector<QString> heads = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg",
};

std::vector<QString> names = {
    "jack",
    "tom",
    "alice",
    "jay",
    "peter",
    "doge",
    "zack",
    "james",
    "curry"
};

std::vector<QString> strs = {
    "hello",
    "im fine",
    "the legend of zelda",
    "dark souls",
    "elden ring",
    "super mario odessy"
};

void ChatDialog::addUserListTest()
{
    for(int i=0;i<13;i++){
//        std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr))); // 随机种子
//        std::uniform_int_distribution<int> distribution(0, 100); // 生成0到100的随机数
//        int randomValue = distribution(generator)*i;
        int randomValue = std::rand() % 100 + 1;

        // 得到随机的头像、名字和消息
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        // 创建一个chatUserWid
        auto* chatUserWid = new ChatUserWid();
        auto userInfo = std::make_shared<UserInfo>(0, names[name_i], names[name_i], heads[head_i], 0, strs[str_i]);
        chatUserWid->setInfo(userInfo);
        // 创建一个QListWidgetItem对象
        QListWidgetItem* item = new QListWidgetItem;

        item->setSizeHint(chatUserWid->sizeHint());
        ui->chatUserList->addItem(item);    // 把item加到chatUserList
        ui->chatUserList->setItemWidget(item, chatUserWid); // 把item设置成自定义的widget
        //item->setFlags(item->flags() & ~Qt::ItemIsSelectable); // 没用
    }
}

void ChatDialog::addUserList()
{
    // 按照好友列表加载聊天记录
    auto friendList = UserMgr::GetInstance()->getChatListPerPage();
    if(!friendList.empty()){
        for(auto& myFriend : friendList){
            auto find_iter = _chat_items_added.find(myFriend->_uid);
            if(find_iter != _chat_items_added.end()){   // 在已添加条目中找到了就跳过
                continue;
            }
            ChatUserWid* chat_user_wid = new ChatUserWid();
            std::shared_ptr<UserInfo> userInfo = std::make_shared<UserInfo>(myFriend);
            chat_user_wid->setInfo(userInfo);

            QListWidgetItem* item = new QListWidgetItem();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chatUserList->addItem(item);
            ui->chatUserList->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(myFriend->_uid, item);
        }
        UserMgr::GetInstance()->updateChatLoadedCount();
    }
}

void ChatDialog::addContactList()
{
    // 按照好友列表加载聊天记录
    auto friendList = UserMgr::GetInstance()->getConListPerPage();
    if(!friendList.empty()){
        for(auto& myFriend : friendList){
            auto find_iter = _contact_items_added.find(myFriend->_uid);
            if(find_iter != _contact_items_added.end()){   // 在已添加条目中找到了就跳过
                continue;
            }
            ContactUserItem* contact_wid = new ContactUserItem();
            contact_wid->setInfo(myFriend->_uid, myFriend->_name, myFriend->_icon);

            QListWidgetItem* item = new QListWidgetItem();
            item->setSizeHint(contact_wid->sizeHint());
            ui->contactList->addItem(item);
            ui->contactList->setItemWidget(item, contact_wid);
            _contact_items_added.insert(myFriend->_uid, item);
        }
        UserMgr::GetInstance()-> updateContactLoadedCount();
    }
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this){
        if(event->type() == QEvent::MouseButtonRelease){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            handleGlobalMousePress(mouseEvent);
        }
    }

    return QDialog::eventFilter(watched, event);
}

void ChatDialog::showList(ChatUIMode mode, bool search)
{
    _mode = mode;
    if(search){                                 // 在多种模式下都有搜索功能
        ui->chatUserList->hide();
        ui->contactList->hide();
        ui->searchList->show();
        _isSearching = true;
    }
    else if(_mode == ChatUIMode::ChatMode){      // 处于聊天模式
        ui->contactList->hide();
        ui->searchList->hide();
        ui->chatUserList->show();
        _isSearching = false;
    }
    else if(_mode == ChatUIMode::ContactMode){   // 处于联系人模式
        ui->searchList->hide();
        ui->chatUserList->hide();
        ui->contactList->show();
        _isSearching = false;
    }
}

void ChatDialog::addLabelGroup(StateWidget *label)
{
    _lbList.push_back(label);
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    // 不在搜索状态直接返回
    if(_isSearching == false) return;

    // 将鼠标的全局坐标转化为相对于searchList的局部坐标（如searchList左上角坐标在屏幕上位置为(100,200)，则posInSearchList为(globalX-100,globalY-200)）
    QPoint posInSearchList = ui->searchList->mapFromGlobal(event->globalPos());
    
    if(!ui->searchList->rect().contains(posInSearchList)){  // contains() 是 QRect 类的方法，用于检查一个点是否在该矩形区域内
        ui->searchEdit->clear();    // 清除搜索框内容
        showList(_mode, false);     // 隐藏搜索列表
    }
}

void ChatDialog::setSelectChatItem(int uid)
{
    if(ui->chatUserList->count() <= 0){
        return;
    }

    if(uid == 0){
        ui->chatUserList->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->chatUserList->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->chatUserList->itemWidget(firstItem);
        if(!widget){
            return;
        }
        // 把widget转成ChatUserWid类型
        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if(!con_item){
            return;
        }
        // 得到当前条目记录的用户uid
        _cur_chat_uid = con_item->getUserInfo()->_uid;

        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        qDebug() << "uid " <<uid<< " not found, set curent row 0";
        ui->chatUserList->setCurrentRow(0);
        return;
    }

    ui->chatUserList->setCurrentItem(find_iter.value());
    _cur_chat_uid = uid;
}

void ChatDialog::setSelectChatPage(int uid)
{
    qDebug()<<"跳转到和用户"<<uid<<"的聊天界面";

    if( ui->chatUserList->count() <= 0){
        return;
    }
    // 如果不传入uid则默认为0，显示最上面一项的聊天信息
    if (uid == 0) {
       auto item = ui->chatUserList->item(0);
       //转为widget
       QWidget* widget = ui->chatUserList->itemWidget(item);
       if (!widget) {
           return;
       }

       auto con_item = qobject_cast<ChatUserWid*>(widget);
       if (!con_item) {
           return;
       }

       //设置信息
       auto user_info = con_item->getUserInfo();
       ui->chatPage->setUserInfo(user_info);
       return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        return;
    }

    //转为widget
    QWidget *widget = ui->chatUserList->itemWidget(find_iter.value());
    if(!widget){
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->getItemType();
    if(itemType == CHAT_USER_ITEM){
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if(!con_item){
            return;
        }

        //设置信息
        auto user_info = con_item->getUserInfo();
        ui->chatPage->setUserInfo(user_info);

        return;
    }
}

void ChatDialog::updateChatMsg(const std::vector<std::shared_ptr<TextChatData> > & msgdata)
{
    for(auto& msg:msgdata){
        // 如果当前选中的聊天好友不是发消息过来的好友就直接跳出
        if(msg->_from_uid != _cur_chat_uid){
            break;
        }
        ui->chatPage->appendChatMsg(msg);
    }
}

void ChatDialog::slot_loading_chat_user()
{
    // 全部加载完了就直接返回
    if(UserMgr::GetInstance()->isLoadChatFin()){
        qDebug()<<"已加载完全部聊天记录";
        return;
    }

    // 创建一个加载对话框，在加载更多信息时显示正在加载中
    LoadingDialog* loadingDlg = new LoadingDialog(this);
    loadingDlg->setModal(true); // 将对话框设置成模态的，必须等对话框处理完才能进行下一步
    loadingDlg->show();
    qDebug()<<"添加更多用户信息...";
    //addUserListTest();  // 这一行为测试用，实际得加载真实数据
    addUserList();

    // 加载完毕后关闭对话框
    loadingDlg->hide();
    loadingDlg->deleteLater();



}

void ChatDialog::slot_loading_contact_user()
{
    // 全部加载完了就直接返回
    if(UserMgr::GetInstance()->isLoadConFin()){
        qDebug()<<"已加载完全部联系人";
        return;
    }

    // 创建一个加载对话框，在加载更多信息时显示正在加载中
    LoadingDialog* loadingDlg = new LoadingDialog(this);
    loadingDlg->setModal(true); // 将对话框设置成模态的，必须等对话框处理完才能进行下一步
    loadingDlg->show();
    qDebug()<<"添加更多用户信息...";
    //addUserListTest();  // 这一行为测试用，实际得加载真实数据
    addContactList();

    // 加载完毕后关闭对话框
    loadingDlg->hide();
    loadingDlg->deleteLater();
}

void ChatDialog::slot_sideWid_clicked(StateWidget *w)
{
    // 处于被选中状态的控件智能有一个，就是发起点击事件的控件，其他控件的状态都要设置成normal
    qDebug()<<"侧边控件点击事件";
    for(auto it=_lbList.begin();it!=_lbList.end();it++){
        if(*it == w){   // 如果碰到自身
            // 由于StateWidget中设置了鼠标按下和释放事件，在发出clicked信号时状态已经为selected了
            // 判断信号来源
            if(w == ui->sideChatLabel){
                ui->stackedWidget->setCurrentWidget(ui->chatPage);  // 如果点击的是sideChatLabel就把右侧切换到聊天页面
                _mode = ChatUIMode::ChatMode;
            }
            else if(w == ui->sideContactLabel){
                ui->stackedWidget->setCurrentWidget(ui->friendApplyPage); // 如果点击的是sideContactLabel就把右侧切换到好友请求页面
                _mode = ChatUIMode::ContactMode;
            }

            showList(_mode);
        }
        else{
            if((*it)->getCurState() == ClickLbState::Selected){ // 找到之前被选中的控件
                // 将其置为普通状态
                (*it)->clearState();
            }
        }
    }
}

void ChatDialog::slot_text_changed(const QString& str)
{
    if(!str.isEmpty()){
        showList(_mode, true);      // 第二个参数为true，展示一下搜索框
    }
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> applyInfo)
{
    int from_uid = applyInfo->_from_uid;
    QString applyName = applyInfo->_name;
    QString desc = applyInfo->_desc;
    QString nick = applyInfo->_nick;
    QString icon = applyInfo->_icon;
    int sex = applyInfo->_sex;
    qDebug()<<"收到好友申请，申请人uid:"<<from_uid<<"，申请人介绍："<<applyName<<"，申请人昵称："<<nick;

    bool b_already = UserMgr::GetInstance()->alreadyApply(from_uid);
    if(b_already){
        qDebug()<<"申请人已申请过好友";
        return;
    }

    UserMgr::GetInstance()->addApplyList(std::make_shared<ApplyInfo>(applyInfo));
    ui->sideContactLabel->showRedPoint(true);           // 侧边栏联系人标签显示小红点
    ui->contactList->showRedPoint(true);
    ui->friendApplyPage->addNewApply(applyInfo);        // 右侧好友申请界面添加新申请条目
}

void ChatDialog::slot_add_auth_firend(std::shared_ptr<AuthInfo> authInfo)
{
    bool isFriend = UserMgr::GetInstance()->checkFriendByUid(authInfo->_uid);
    if(isFriend){
        return;
    }

    auto* chat_user_wid = new ChatUserWid;
    auto user_info = std::make_shared<UserInfo>(authInfo);
    chat_user_wid->setInfo(user_info);

    auto* item = new QListWidgetItem;
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chatUserList->insertItem(0, item);
    ui->chatUserList->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(authInfo->_uid, item);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> rsp)
{
    bool isFriend = UserMgr::GetInstance()->checkFriendByUid(rsp->_uid);
    if(isFriend){
        return;
    }

    auto* chat_user_wid = new ChatUserWid;
    auto user_info = std::make_shared<UserInfo>(rsp);
    chat_user_wid->setInfo(user_info);

    auto* item = new QListWidgetItem;
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chatUserList->insertItem(0, item);
    ui->chatUserList->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(rsp->_uid, item);
}

void ChatDialog::slot_jump_chat_item_from_search(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot_jump_chat_item called " << endl;
    auto find_iter = _chat_items_added.find(si->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chatUserList->scrollToItem(find_iter.value());
        ui->sideChatLabel->setSelected(true);
        // 设置选中的聊天条目
        setSelectChatItem(si->_uid);
        //更新聊天界面信息
        setSelectChatPage(si->_uid);
        // 模拟点击了一下测边框聊天标签，跳转到聊天界面
        slot_sideWid_clicked(ui->sideChatLabel);
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->setInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chatUserList->insertItem(0, item);
    ui->chatUserList->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(si->_uid, item);

    ui->sideChatLabel->setSelected(true);
    // 设置选中的聊天条目
    setSelectChatItem(si->_uid);
    //更新聊天界面信息
    setSelectChatPage(si->_uid);
    // 模拟点击了一下测边框聊天标签，跳转到聊天界面
    slot_sideWid_clicked(ui->sideChatLabel);
}

void ChatDialog::slot_jump_chat_item_from_infoPage(std::shared_ptr<UserInfo> userInfo)
{
    qDebug() << "slot_jump_chat_item called " << endl;
    auto find_iter = _chat_items_added.find(userInfo->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << userInfo->_uid;
        ui->chatUserList->scrollToItem(find_iter.value());
        ui->sideChatLabel->setSelected(true);
        // 设置选中的聊天条目
        setSelectChatItem(userInfo->_uid);
        //更新聊天界面信息
        setSelectChatPage(userInfo->_uid);
        // 模拟点击了一下测边框聊天标签，跳转到聊天界面
        slot_sideWid_clicked(ui->sideChatLabel);
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    chat_user_wid->setInfo(userInfo);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chatUserList->insertItem(0, item);
    ui->chatUserList->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(userInfo->_uid, item);

    ui->sideChatLabel->setSelected(true);
    // 设置选中的聊天条目
    setSelectChatItem(userInfo->_uid);
    //更新聊天界面信息
    setSelectChatPage(userInfo->_uid);
    // 模拟点击了一下测边框聊天标签，跳转到聊天界面
    slot_sideWid_clicked(ui->sideChatLabel);
}

void ChatDialog::slot_switch_friend_info_page(std::shared_ptr<UserInfo> userInfo)
{
    _lastWidget = ui->friendInfoPage;
    ui->stackedWidget->setCurrentWidget(ui->friendInfoPage);
    ui->friendInfoPage->setInfo(userInfo);
}

void ChatDialog::slot_switch_apply_friend_page()
{
    _lastWidget = ui->friendApplyPage;
    ui->stackedWidget->setCurrentWidget(ui->friendApplyPage);
}

void ChatDialog::slot_chatList_item_clicked(QListWidgetItem *item)
{
    qDebug()<<"slot_chatList_item_clicked called";
    QWidget* widget = ui->chatUserList->itemWidget(item);
    if(widget == nullptr){
        return;
    }
    ListItemBase* baseWid = qobject_cast<ListItemBase*>(widget);
    if(baseWid == nullptr){
        return;
    }
    // 判断widget类型
    auto itemType = baseWid->getItemType();
    if(itemType == ListItemType::CHAT_USER_ITEM){
        ChatUserWid* chatWid = qobject_cast<ChatUserWid*>(baseWid);
        if(chatWid == nullptr){
            return;
        }
        //切换对应的聊天界面
        auto userInfo = chatWid->getUserInfo();
        ui->chatPage->setUserInfo(userInfo);
        _cur_chat_uid = userInfo->_uid;
        return;
    }
    
    
}

void ChatDialog::slot_append_send_chat_msg(std::shared_ptr<TextChatData> chatData)
{
    if(_cur_chat_uid==0){
        return;
    }
    auto find_iter = _chat_items_added.find(_cur_chat_uid);
    if(find_iter == _chat_items_added.end()){
        return;
    }
    // 得到聊天列表中对应的QWidget
    QWidget* widget = ui->chatUserList->itemWidget(find_iter.value());
    if(!widget){
        return;
    }
    ListItemBase* itemBase = qobject_cast<ListItemBase*>(widget);
    if(!itemBase){
        return;
    }
    auto itemType = itemBase->getItemType();
    if(itemType == ListItemType::CHAT_USER_ITEM){
        auto chatItem = qobject_cast<ChatUserWid*>(itemBase);
        auto userInfo = chatItem->getUserInfo();
        // 往对应用户的_chat_msgs添加当前chatData，保存历史消息
        userInfo->_chat_msgs.push_back(chatData);
        // 更新friendInfo中对应的消息
        std::vector<std::shared_ptr<TextChatData>> msg_vec;
        msg_vec.push_back(chatData);
        UserMgr::GetInstance()->appendFriendChatMsg(_cur_chat_uid, msg_vec);
        return;
    }
}

void ChatDialog::slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg)
{
    auto find_iter = _chat_items_added.find(msg->_from_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "set chat item msg, uid is " << msg->_from_uid;
        QWidget *widget = ui->chatUserList->itemWidget(find_iter.value());
        auto chat_wid = qobject_cast<ChatUserWid*>(widget);
        if(!chat_wid){
            return;
        }
        chat_wid->updateLastMsg(msg->_chat_msgs);
        //更新当前聊天页面记录,如果当前不是和对应好友的聊天就直接返回
        updateChatMsg(msg->_chat_msgs);
        // 更新对应的FriendInfo里的聊天记录
        UserMgr::GetInstance()->appendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    //查询好友信息
    auto fi_ptr = UserMgr::GetInstance()->getFriendByUid(msg->_from_uid);
    auto userInfo = std::make_shared<UserInfo>(fi_ptr);
    chat_user_wid->setInfo(userInfo);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    chat_user_wid->updateLastMsg(msg->_chat_msgs);
     UserMgr::GetInstance()->appendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
    ui->chatUserList->insertItem(0, item);
    ui->chatUserList->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(msg->_from_uid, item);
}
